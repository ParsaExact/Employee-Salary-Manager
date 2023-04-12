#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

typedef vector<vector<string>> WORDS;

const string LEVEL[] = {"team-lead", "senior", "expert", "junior"};
const string EMPLOYEE_FILE = "employees.csv";
const string WORKING_HOUR_FILE = "working_hours.csv";
const string TEAMS_FILE = "teams.csv";
const string SALARY_FILE = "salary_configs.csv";

const string ERROR_LEVEL = "INVALID_LEVEL";
const string ERROR_EMPLOYEE = "EMPLOYEE_NOT_FOUND";
const string ERROR_ARGUMENTS = "INVALID_ARGUMENTS";
const string ERROR_INTERVAL = "INVALID_INTERVAL";
const string DONE_SUCCESSFULLY_MESSAGE = "OK";

const char COMMA = ',';
const char DASH = '-';
const char DOLLAR = '$';
const char SPACE = ' ';

const int NOT_FOUND = -1;
const int INITIAL_BOUNUS = 0;

vector<string> split_words(string line, char del);

class Team
{
public:
    int get_head_id() { return team_head_id; }
    vector<int> get_mem_ids() { return member_ids; }
    void update_bonus();
    Team(string ti, int thi, vector<int> mmids, int bmwh, double bwhmv)
        : team_id(ti), team_head_id(thi), member_ids(mmids), bonus_min_working_hours(bmwh), bonus_working_hour_max_variance(bwhmv), bounus(INITIAL_BOUNUS) {}
    void print_team(); // done

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
    void print_working_hour();                                                                     // done
    void add_working_hour(int id, int day, int start, int end, vector<WorkingHour> working_hours); // done check needed
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
    void print_salary(); // done
    void update_level(vector<string> changes);
    void update_base_salary(string new_base_salary);
    void update_salary_per_hour(string new_salary_per_hour);
    void update_salary_per_extra_hour(string new_salary_per_extra_hour);
    void update_official_working_hours(string new_official_working_hours);
    void update_tax_percantage(string new_tax_percantage);
    string get_level() { return level; }
    int get_base_salary() { return base_salary; }
    int get_salary_per_hour() { return salary_per_hour; }
    int get_salary_per_extra_hour() { return salary_per_extra_hour; }
    int get_official_working_hours() { return official_working_hours; }
    int get_tax_percantage() { return tax_percantage; }

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
    void print_file(); // done
    int get_id() { return id; }
    void match_to_team(Team* emp_team) { employee_team = emp_team; }
    int calculate_salary();
    int calculate_working_hours();
    int calculate_tax();
    int calculate_bonus();
    int calculate_total_earning();
    void add_working_hour(WorkingHour *hour);
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
    pair<int, int> convert_interval_to_pair(string s);
    WORDS read_file(string filename);
    vector<Employee> read_employee_file();
    vector<WorkingHour *> read_working_hour_file();
    vector<int> convert_member_ids(string s);
    vector<Team> read_teams_file();
    vector<Salary> read_salary_file();
};

class OutputFiles
{
public:
    void show_salary_config(vector<Salary> salary, string level);
};

class HandleCommand
{
public:
    void load_data();
    void get_command();
    void initial_working_hours();
    void update_salary_config(vector<string> command_words);
    void add_working_hour();

private:
    void match_employees_and_teams();
    int find_level(string name);
    int find_employee(int id);
    vector<Employee> employees;
    vector<WorkingHour *> working_hours;
    vector<Salary> salaries;
    vector<Team> teams;
};
/////////////////////////////////////////////////////////////////////////

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

void Team ::print_team()
{
    cout << team_id << ' ' << team_head_id << ' ' << endl;
    for (auto x : member_ids)
        cout << x << ' ';
    cout << endl
         << bonus_min_working_hours << ' ' << bonus_working_hour_max_variance;
}

void WorkingHour ::print_working_hour()
{
    cout << employee_id << ' ' << day << ' ' << working_hour.first << ' ' << working_hour.second << endl;
}

void WorkingHour ::add_working_hour(int id, int day, int start, int end, vector<WorkingHour> working_hours)
{
    bool found = true;
    for (auto x : working_hours)
        if (x.employee_id == id)
            found = false;
    if (found)
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
        if (((start >= x.working_hour.first && start <= x.working_hour.second) || (end >= x.working_hour.first && end <= x.working_hour.second)) && x.day == day)
        {
            cout << ERROR_ARGUMENTS << endl;
            return;
        }
    pair<int, int> interval = {start, end};
    WorkingHour(id, day, interval);
}

void Salary ::print_salary()
{
    cout << "Base Salary: " << base_salary << endl
         << "Salary Per Hour: " << salary_per_hour << endl
         << "Salary Per Extra Hour: " << salary_per_extra_hour << endl
         << "Official Working Hours: " << official_working_hours << endl
         << "Tax: " << tax_percantage << '%' << endl;
}

void Salary ::update_base_salary(string new_base_salary)
{
    if(new_base_salary == "-")
        return;
    base_salary = stoi(new_base_salary);
}

void Salary ::update_salary_per_hour(string new_salary_per_hour)
{
    if(new_salary_per_hour == "-")
        return;
    salary_per_hour = stoi(new_salary_per_hour);
}

void Salary ::update_salary_per_extra_hour(string new_salary_per_extra_hour)
{
    if(new_salary_per_extra_hour == "-")
        return;
    salary_per_extra_hour = stoi(new_salary_per_extra_hour);
}

void Salary ::update_official_working_hours(string new_official_working_hours)
{
    if(new_official_working_hours == "-")
        return;
    official_working_hours = stoi(new_official_working_hours);
}

void Salary ::update_tax_percantage(string new_tax_percantage)
{
    if(new_tax_percantage == "-")
        return;
    tax_percantage = stoi(new_tax_percantage);
}

void Salary ::update_level(vector<string> changes)
{
    update_base_salary(changes[2]);
    update_salary_per_hour(changes[3]);
    update_salary_per_extra_hour(changes[4]);
    update_official_working_hours(changes[5]);
    update_tax_percantage(changes[6]);
    cout << DONE_SUCCESSFULLY_MESSAGE << endl;
}

void Employee ::print_file()
{
    cout << id << ' ' << name << ' ' << age << ' ' << level << endl;
    for (WorkingHour *i : working_days)
        i->print_working_hour();
    cout << endl;
}

void Employee ::add_working_hour(WorkingHour *hour)
{
    working_days.push_back(hour);
}

pair<int, int> ReadFiles ::convert_interval_to_pair(string s)
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

WORDS ReadFiles ::read_file(string filename)
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

vector<Employee> ReadFiles ::read_employee_file()
{
    WORDS employee_items = read_file(EMPLOYEE_FILE);
    vector<Employee> allEmp;
    for (int i = 0; i < employee_items.size(); i++)
    {
        allEmp.push_back(Employee(stoi(employee_items[i][0]), employee_items[i][1], stoi(employee_items[i][2]), employee_items[i][3]));
    }
    return allEmp;
}

vector<WorkingHour *> ReadFiles ::read_working_hour_file()
{
    WORDS working_hour_items = read_file(WORKING_HOUR_FILE);
    vector<WorkingHour *> allWorkingHour;
    for (int i = 0; i < working_hour_items.size(); i++)
    {
        allWorkingHour.push_back(new WorkingHour(stoi(working_hour_items[i][0]), stoi(working_hour_items[i][1]),
                                                 convert_interval_to_pair(working_hour_items[i][2])));
    }
    return allWorkingHour;
}

vector<int> ReadFiles ::convert_member_ids(string s)
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

vector<Team> ReadFiles ::read_teams_file()
{
    WORDS team_ids = read_file(TEAMS_FILE);
    vector<Team> allTeamItems;
    for (int i = 0; i < team_ids.size(); i++)
    {
        allTeamItems.push_back(Team((team_ids[i][0]), stoi(team_ids[i][1]), convert_member_ids(team_ids[i][2]), stoi(team_ids[i][3]), stod(team_ids[i][4])));
    }
    return allTeamItems;
}

vector<Salary> ReadFiles ::read_salary_file()
{
    WORDS salary_items = read_file(SALARY_FILE);
    vector<Salary> allSalaryItems;
    for (int i = 0; i < salary_items.size(); i++)
    {
        allSalaryItems.push_back(Salary((salary_items[i][0]), stoi(salary_items[i][1]), stoi(salary_items[i][2]), stoi(salary_items[i][3]), stoi(salary_items[i][4]), stoi(salary_items[i][5])));
    }
    return allSalaryItems;
}

void OutputFiles ::show_salary_config(vector<Salary> salary, string level)
{
    bool not_found = true;
    for (auto x : salary)
        if (x.get_level() == level)
        {
            x.print_salary();
            not_found = false;
        }
    if (not_found)
        cout << ERROR_LEVEL << endl;
}

int HandleCommand ::find_level(string level_name)
{
    for (int i = 0; i < salaries.size(); i++)
    {
        if (salaries[i].get_level() == level_name)
            return i;
    }
    return NOT_FOUND;
}

void HandleCommand ::update_salary_config(vector<string> command_words)
{
    int salary_index = find_level(command_words[1]);
    if (salary_index != NOT_FOUND)
    {
        salaries[salary_index].update_level(command_words);
    }
    else
        cout << ERROR_LEVEL << endl;
}

void HandleCommand ::get_command()
{
    OutputFiles the_output;
    string command, line;
    while (getline(cin, line))
    {
        vector<string> command_words = split_words(line, SPACE);
        command = command_words[0];
        if (command == "show_salary_config")
        {
            the_output.show_salary_config(salaries, command_words[1]);
        }
        if (command == "add_working_hours")
        {
            // add_working_hour(command_words);
        }
        if (command == "update_salary_config")
            update_salary_config(command_words);
    }
}

void HandleCommand ::match_employees_and_teams()
{
    vector<int> mem_ids;
    int employee_index;
    for(int i = 0; i < teams.size(); i++)
    {
        mem_ids = teams[i].get_mem_ids();
        employee_index = find_employee(teams[i].get_head_id());
        employees[employee_index].match_to_team(&teams[i]);
        for(int j = 0; j < mem_ids.size(); j++)
        {
            employee_index = find_employee(mem_ids[j]);
            employees[employee_index].match_to_team(&teams[i]);
        }
    }
}

void HandleCommand ::load_data()
{
    ReadFiles my_input;
    employees = my_input.read_employee_file();
    working_hours = my_input.read_working_hour_file();
    teams = my_input.read_teams_file();
    salaries = my_input.read_salary_file();
    initial_working_hours();
    match_employees_and_teams();
}

int HandleCommand ::find_employee(int id)
{
    for (int i = 0; i < employees.size(); i++)
    {
        if (employees[i].get_id() == id)
            return i;
    }
    return NOT_FOUND;
}

void HandleCommand ::initial_working_hours()
{
    for (WorkingHour *hour : working_hours)
    {
        int employee_index = find_employee(hour->get_id());
        employees[employee_index].add_working_hour(hour);
    }
}

int main()
{
    HandleCommand program;
    program.load_data();
    program.get_command();
}