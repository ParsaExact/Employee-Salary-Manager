#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

typedef vector<vector<string>> WORDS;

WORDS read_file(string filename);

const string LEVEL[] = {"team-lead", "senior", "expert", "junior"};
const string EMPLOYEE_FILE = "employees.csv";
const string WORKING_HOUR_FILE = "working_hours.csv";
const string TEAMS_FILE = "teams.csv";
const string SALARY_FILE = "salary_configs.csv";

const string ERROR_LEVEL = "INVALID_LEVEL";
const string ERROR_EMPLOYEE = "EMPLOYEE_NOT_FOUND";
const string ERROR_ARGUMENTS = "INVALID_ARGUMENTS";
const string ERROR_INTERVAL = "INVALID_INTERVAL";

const char COMMA = ',';
const char DASH = '-';
const char DOLLAR = '$';
const char SPACE = ' ';

class Team
{
public:
    void set_bonus();
    void update_bonus();
    Team(string ti, int thi, vector<int> mmids, int bmwh, double bwhmv)
        : team_id(ti), team_head_id(thi), member_ids(mmids), bonus_min_working_hours(bmwh), bonus_working_hour_max_variance(bwhmv) {}
    void print_team()
    {
        cout << team_id << ' ' << team_head_id << ' ' << endl;
        for (auto x : member_ids)
            cout << x << ' ';
        cout << endl
             << bonus_min_working_hours << ' ' << bonus_working_hour_max_variance;
    }

private:
    string team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    double bonus_working_hour_max_variance;
    int bounus;
};
class WorkingHour
{
public:
    WorkingHour(int i, int d, pair<int, int> wh)
        : employee_id(i), day(d), working_hour(wh) {}
    int get_id() { return employee_id; }
    int calculate_duration();
    void print_working_hour()
    {
        cout << employee_id << ' ' << day << ' ' << working_hour.first << ' ' << working_hour.second << endl;
    }
    void add_working_hour(int id, int day, int start, int end, vector<WorkingHour> working_hours)
    {
        bool flag = true;
        for (auto x : working_hours)
            if (x.employee_id == id)
                flag = false;
        if (flag)
        {
            cout << ERROR_EMPLOYEE << endl;
            return;
        }
        if (day > 30 || day < 1 || start >= end)
        {
            cout << ERROR_ARGUMENTS << endl;
            return;
        }
        for (auto x : working_hours)
            if (((start >= x.working_hour.first && start <= x.working_hour.second) || (end >= x.working_hour.first && end <= x.working_hour.second))&& x.day==day)
            {
                cout << ERROR_ARGUMENTS << endl;
                return;
            }
        pair<int,int> interval = {start,end};
        WorkingHour(id,day,interval);
    }

private:
    int employee_id;
    int day;
    pair<int, int> working_hour;
};
class Salary
{
public:
    Salary(string l, int bs, int sph, int speh, int owh, int tp)
        : level(l), base_salary(bs), salary_per_hour(sph), salary_per_extra_hour(speh), official_working_hours(owh), tax_percantage(tp) {}
    void print_salary()
    {
        cout << "Base Salary: " << base_salary << endl
             << "Salary Per Hour: " << salary_per_hour << endl
             << "Salary Per Extra Hour: " << salary_per_extra_hour << endl
             << "Official Working Hours: " << official_working_hours << endl
             << "Tax: " << tax_percantage << '%' << endl;
    }
    void update_level();
    void update_base_salary();
    void update_salary_per_hour();
    void update_salary_per_extra_hour();
    void update_official_working_hours();
    void update_tax_percantage();
    string get_level() { return level; }
    int get_base_salary();
    int get_salary_per_hour();
    int get_salary_per_extra_hour();
    int get_official_working_hours();
    int get_tax_percantage();

private:
    string level;
    int base_salary;
    int salary_per_hour, salary_per_extra_hour;
    int official_working_hours;
    int tax_percantage;
};
class Employee
{
public:
    Employee(int i, string n, int a, string l)
        : id(i), name(n), age(a), level(l) {}
    void print_file()
    {
        cout << id << ' ' << name << ' ' << age << ' ' << level << endl;
    }
    int calculate_salary();
    int calculate_working_hours();
    int calculate_tax();
    int calculate_bonus();
    int calculate_total_earning();
    void add_working_hour(pair<int, int> interval, int id, vector<WorkingHour> wh){

    };
    void delete_working_hours();

private:
    vector<WorkingHour *> working_days;
    Team *employee_team;
    string name;
    int age;
    string level;
    int id;
    string team_id;
};

class ReadFiles
{
public:
    ReadFiles() {}
    pair<int, int> convert_interval_to_pair(string s)
    {
        pair<int, int> interval;
        int first, second;
        for (int i = 0; i < s.size(); i++)
            if (s[i] == DASH)
            {
                first = stoi(s.substr(0, i));
                second = stoi(s.substr(i + 1, s.size() - 1));
            }
        interval = {first, second};
        return interval;
    }
    vector<string> split_words(string line, char del)
    {
        vector<string> words_in_line;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == del)
            {
                words_in_line.push_back(line.substr(0, i));
                line = line.substr(i + 1, line.size());
                i = 0;
            }
            if (i == line.size() - 1)
            {
                words_in_line.push_back(line.substr(0));
                line = line.substr(i + 1, line.size());
                i = 0;
            }
        }
        return words_in_line;
    }
    WORDS read_file(string filename)
    {
        ifstream file;
        file.open(filename);
        string line;
        WORDS all_words;
        getline(file, line);
        while (getline(file, line))
        {
            vector<string> words = split_words(line, COMMA);
            all_words.push_back(words);
        }
        file.close();
        return all_words;
    }
    vector<Employee> read_employee_file()
    {
        WORDS employee_items = read_file(EMPLOYEE_FILE);
        vector<Employee> allEmp;
        for (int i = 0; i < employee_items.size(); i++)
        {
            allEmp.push_back(Employee(stoi(employee_items[i][0]), employee_items[i][1], stoi(employee_items[i][2]), employee_items[i][3]));
        }
        return allEmp;
    }

    vector<WorkingHour> read_working_hour_file()
    {
        WORDS working_hour_items = read_file(WORKING_HOUR_FILE);
        vector<WorkingHour> allWorkingHour;
        for (int i = 0; i < working_hour_items.size(); i++)
        {
            allWorkingHour.push_back(WorkingHour(stoi(working_hour_items[i][0]), stoi(working_hour_items[i][1]), convert_interval_to_pair(working_hour_items[i][2])));
        }
        return allWorkingHour;
    }
    vector<int> convert_member_ids(string s)
    {
        vector<int> member_ids;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == DOLLAR)
            {
                member_ids.push_back(stoi(s.substr(0, i + 1)));
                s = s.substr(i + 1, s.size());
                i = 0;
            }
            if (i == s.size() - 1)
            {
                member_ids.push_back(stoi(s.substr(0, i + 1)));
                s = s.substr(i + 1, s.size());
                i = 0;
            }
        }
        return member_ids;
    }
    vector<Team> read_teams_file()
    {
        WORDS team_ids = read_file(TEAMS_FILE);
        vector<Team> allTeamItems;
        for (int i = 0; i < team_ids.size(); i++)
        {
            allTeamItems.push_back(Team((team_ids[i][0]), stoi(team_ids[i][1]), convert_member_ids(team_ids[i][2]), stoi(team_ids[i][3]), stod(team_ids[i][4])));
        }
        return allTeamItems;
    }
    vector<Salary> read_salary_file()
    {
        WORDS salary_items = read_file(SALARY_FILE);
        vector<Salary> allSalaryItems;
        for (int i = 0; i < salary_items.size(); i++)
        {
            allSalaryItems.push_back(Salary((salary_items[i][0]), stoi(salary_items[i][1]), stoi(salary_items[i][2]), stoi(salary_items[i][3]), stoi(salary_items[i][4]), stoi(salary_items[i][5])));
        }
        return allSalaryItems;
    }
};
class OutputFiles
{
public:
    OutputFiles() {}
    void show_salary_config(vector<Salary> salary, string level)
    {
        bool flag = true;
        for (auto x : salary)
            if (x.get_level() == level)
            {
                x.print_salary();
                flag = false;
            }
        if (flag)
            cout << ERROR_LEVEL << endl;
    }
};

int main()
{
    ReadFiles my_input;
    OutputFiles my_output;
    vector<Employee> employees;
    employees = my_input.read_employee_file();
    // for (auto x : employees)
    //     x.print_file();

    vector<WorkingHour> working_hour;
    working_hour = my_input.read_working_hour_file();
    // for (auto x : working_hour)
    //     x.print_working_hour();

    vector<Team> teams = my_input.read_teams_file();
    // for (auto x : teams)
    //     x.print_team();

    vector<Salary> salary = my_input.read_salary_file();
    // for (auto x : salary)
    //     x.print_salary();

    string line;
    while (getline(cin, line))
    {
        vector<string> commands = my_input.split_words(line, SPACE);
        string command = commands[0];
        if (command == "show_salary_config")
        {
            my_output.show_salary_config(salary, commands[1]);
        }
        if (command == "add_working_hours")
        {
            //todo
        }
    }
    return 0;
}