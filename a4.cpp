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
const string EMPLOYEE_WITH_NO_TEAM = "N/A";
const string ERROR_TEAM = "TEAM_NOT_FOUND";

const char COMMA = ',';
const char DASH = '-';
const char DOLLAR = '$';
const char SPACE = ' ';

const int NUMBER_OF_MONTH_DAYS = 30;
const int NOT_FOUND = -1;
const int INITIAL_BOUNUS = 0;

vector<string> split_words(string line, char del);

class Team
{
public:
    int get_head_id() { return team_head_id; }
    string get_team_id() { return team_id; }
    void set_team_id(string _team_id) { team_id = _team_id; }
    vector<int> get_mem_ids() { return member_ids; }
    void update_bonus(int bonus_perc) { bonus = bonus_perc; };
    Team(string ti, int thi, vector<int> mmids, int bmwh, double bwhmv)
        : team_id(ti), team_head_id(thi), member_ids(mmids), bonus_min_working_hours(bmwh),
          bonus_working_hour_max_variance(bwhmv), bonus(INITIAL_BOUNUS) {}
    void print_team();
    int get_bonus() { return bonus; }

private:
    string team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    double bonus_working_hour_max_variance;
    int bonus;
};

class WorkingHour
{
public:
    WorkingHour(int i, int d, pair<int, int> wh)
        : employee_id(i), day(d), working_hour(wh) {}
    int get_id() { return employee_id; }
    int get_day() { return day; }
    pair<int, int> get_pair() { return working_hour; }
    int calculate_duration() { return working_hour.second - working_hour.first; };
    void print_working_hour();                                            // done
    bool available_add_working_hour(int id, int day, int start, int end); // done check needed
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
        : id(i), name(n), age(a), level_name(l), employee_team(NULL) {}
    void print_file();
    int get_id() { return id; }
    void set_level(Salary* new_level) { level = new_level; }
    Salary* get_level() {return level;}
    string get_level_name() { return level_name; }
    void match_to_team(Team *emp_team) { employee_team = emp_team; }
    int calculate_salary();
    int calculate_working_hours();
    int calculate_tax();
    int calculate_bonus();
    int calculate_total_earning();
    int calculate_absent_days();
    void add_working_hour(WorkingHour *hour);
    void delete_working_hours(int day);
    void show_personal_info();
    void show_working_info();

private:
    bool day_counted_before(vector<int> counted_present_days, int day);
    vector<WorkingHour *> working_days;
    Team *employee_team;
    string name;
    int age;
    string level_name;
    int id;
    Salary* level;
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

class HandleCommand
{
public:
    void load_data();
    void get_command();
    void initial_working_hours();
    void update_salary_config(vector<string> command_words);
    void add_working_hour(vector<string> command_words);
    void report_total_hours_per_day(vector<string> commands);
    void update_team_bonus(vector<string> commands);
    void report_employee_per_hour(vector<string> command);
    void delete_working_hours(vector<string> command_words);
    void report_employee_salary(vector<string> command_words);
    void show_salary_config(string level);
    int find_level(string name);
private:
    void match_employees_and_levels();
    void free_trash_data(int day, int id);
    bool available_add_request(int id, int day, int start, int end);
    void match_employees_and_teams();
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
            words_in_line.push_back(line);
        }
    }
    return words_in_line;
}

void Team ::print_team()
{
    cout << team_id << ' ' << team_head_id << ' ' << endl;
    cout << "Bonus:" << bonus << endl;
    for (auto x : member_ids)
        cout << x << ' ';
    cout << endl
         << bonus_min_working_hours << ' ' << bonus_working_hour_max_variance;
}

void WorkingHour ::print_working_hour()
{
    cout << employee_id << ' ' << day << ' ' << working_hour.first << ' ' << working_hour.second << endl;
}

bool WorkingHour ::available_add_working_hour(int id, int new_day, int start, int end)
{
    if ((((start >= working_hour.first && start < working_hour.second) || (end > working_hour.first &&
                                                                           end <= working_hour.second)) ||
         (start <= working_hour.first && end >= working_hour.second)) &&
        new_day == day && id == employee_id)
    {
        cout << ERROR_INTERVAL << endl;
        return false;
    }
    return true;
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
    if (new_base_salary == "-")
        return;
    base_salary = stoi(new_base_salary);
}

void Salary ::update_salary_per_hour(string new_salary_per_hour)
{
    if (new_salary_per_hour == "-")
        return;
    salary_per_hour = stoi(new_salary_per_hour);
}

void Salary ::update_salary_per_extra_hour(string new_salary_per_extra_hour)
{
    if (new_salary_per_extra_hour == "-")
        return;
    salary_per_extra_hour = stoi(new_salary_per_extra_hour);
}

void Salary ::update_official_working_hours(string new_official_working_hours)
{
    if (new_official_working_hours == "-")
        return;
    official_working_hours = stoi(new_official_working_hours);
}

void Salary ::update_tax_percantage(string new_tax_percantage)
{
    if (new_tax_percantage == "-")
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

void Employee ::delete_working_hours(int day)
{
    for (int i = 0; i < working_days.size(); i++)
    {
        if (working_days[i]->get_day() == day)
        {
            working_days.erase(working_days.begin() + i);
            i--;
        }
    }
}

void Employee ::show_personal_info()
{
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Level: " << level_name << endl;
    if (employee_team != NULL)
        cout << "Team ID: " << employee_team->get_team_id() << endl;
    else
        cout << "Team ID: " << EMPLOYEE_WITH_NO_TEAM << endl;
}

int Employee ::calculate_working_hours()
{
    int total_working_hours = 0;
    for (int i = 0; i < working_days.size(); i++)
    {
        total_working_hours += working_days[i]->calculate_duration();
    }
    return total_working_hours;
}

bool Employee ::day_counted_before(vector<int> counted_present_days, int day)
{
    for (int i = 0; i < counted_present_days.size(); i++)
    {
        if (counted_present_days[i] == day)
            return true;
    }
    return false;
}

int Employee ::calculate_absent_days()
{
    int present_days = 0;
    vector<int> counted_present_days;
    for (int i = 0; i < working_days.size(); i++)
    {
        if (!day_counted_before(counted_present_days, working_days[i]->get_day()))
        {
            present_days++;
            counted_present_days.push_back(working_days[i]->get_day());
        }
    }
    return NUMBER_OF_MONTH_DAYS - present_days;
}

int Employee ::calculate_salary()
{
    int total_working_hours = calculate_working_hours();
    if (total_working_hours <= level->get_official_working_hours())
        return level->get_base_salary() + total_working_hours * level->get_salary_per_hour();

    return level->get_base_salary() + level->get_official_working_hours() * level->get_salary_per_hour() +
           (total_working_hours - level->get_official_working_hours()) * level->get_salary_per_extra_hour();
}

int Employee ::calculate_bonus()
{
    int bonus = calculate_salary() * employee_team->get_bonus() / 100;
    return bonus;
}

int Employee::calculate_tax()
{
    int tax = (calculate_salary() + calculate_bonus()) * level->get_tax_percantage() / 100;
    return tax;
}

int Employee ::calculate_total_earning()
{
    int total_earn = calculate_salary() + calculate_bonus() - calculate_tax();
    return total_earn;
}

void Employee ::show_working_info()
{
    cout << "Total Working Hours: " << calculate_working_hours() << endl;
    cout << "works till here" << endl;
    cout << "Absent Days: " << calculate_absent_days() << endl;
    cout << "Salary: " << calculate_salary() << endl;
    if (employee_team == NULL)
        cout << "Bonus: "
             << "0" << endl;
    else
        cout << "Bonus: " << calculate_bonus() << endl;
    cout << "Tax: " << calculate_tax() << endl;
    cout << "Total Earning: " << calculate_total_earning() << endl;
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
        allEmp.push_back(Employee(stoi(employee_items[i][0]), employee_items[i][1], stoi(employee_items[i][2]),
                                  employee_items[i][3].substr(0,employee_items[i][3].size() - 1)));
    }
    return allEmp;
}

vector<WorkingHour *> ReadFiles ::read_working_hour_file()
{
    WORDS working_hour_items = read_file(WORKING_HOUR_FILE);
    vector<WorkingHour *> allWorkingHour;
    for (int i = 0; i < working_hour_items.size(); i++)
    {
        for (int j = 1; j < working_hour_items[i].size(); j += 2)
            allWorkingHour.push_back(new WorkingHour(stoi(working_hour_items[i][0]), stoi(working_hour_items[i][j]),
                                                     convert_interval_to_pair(working_hour_items[i][j + 1])));
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
        allTeamItems.push_back(Team((team_ids[i][0]), stoi(team_ids[i][1]), convert_member_ids(team_ids[i][2]),
                                    stoi(team_ids[i][3]), stod(team_ids[i][4])));
        ;
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

void HandleCommand ::show_salary_config(string level)
{
    int salary_index = find_level(level);
    if (salary_index == NOT_FOUND)
        cout << ERROR_LEVEL << endl;
    else
        salaries[salary_index].print_salary();
}

int HandleCommand ::find_level(string level_name)
{
    for (int i = 0; i < salaries.size(); i++)
    {
        if (salaries[i].get_level() == level_name)
        {
            return i;
        }
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

bool HandleCommand ::available_add_request(int id, int day, int start, int end)
{
    if (day > 30 || day < 1 || start >= end)
    {
        cout << ERROR_ARGUMENTS << endl;
        return false;
    }
    if (start < 0 || start > 24 || end > 24 || end < 0)
    {
        cout << ERROR_ARGUMENTS << endl;
        return false;
    }
    for (WorkingHour *x : working_hours)
        if (!(x->available_add_working_hour(id, day, start, end)))
            return false;
    return true;
}

void HandleCommand ::add_working_hour(vector<string> command_words)
{
    int employee_index = find_employee(stoi(command_words[1]));
    if (employee_index == NOT_FOUND)
        cout << ERROR_EMPLOYEE << endl;
    else
    {
        if (available_add_request(stoi(command_words[1]), stoi(command_words[2]),
                                  stoi(command_words[3]), stoi(command_words[4])))
        {
            pair<int, int> new_duration = {stoi(command_words[3]), stoi(command_words[4])};
            working_hours.push_back(new WorkingHour(stoi(command_words[1]), stoi(command_words[2]), new_duration));
            employees[employee_index].add_working_hour(working_hours[working_hours.size() - 1]);
            cout << DONE_SUCCESSFULLY_MESSAGE << endl;
        }
    }
}

void HandleCommand::report_total_hours_per_day(vector<string> commands)
{
    int start = stoi(commands[1]), end = stoi(commands[2]);
    vector<pair<int, int>> sum_of_each_day;
    for (int i = start; i <= end; i++)
    {
        int sum = 0;
        for (auto x : working_hours)
            if (x->get_day() == i)
                sum += x->calculate_duration();
        sum_of_each_day.push_back({sum, i});
        cout << "Day #" << i << ": " << sum << endl;
    }
    cout << "---" << endl;
    vector<int> mins, maxs;
    for (int i = 0; i < sum_of_each_day.size(); i++)
    {
        bool is_max = false, is_min = false;
        for (int j = 0; j < sum_of_each_day.size(); j++)
        {
            if (i != j && sum_of_each_day[i].first > sum_of_each_day[j].first)
                is_max = true;
            if (i != j && sum_of_each_day[i].first < sum_of_each_day[j].first)
                is_min = true;
        }
        if (is_min && !is_max)
            mins.push_back(sum_of_each_day[i].second);
        if (is_max && !is_min)
            maxs.push_back(sum_of_each_day[i].second);
    }
    cout << "Day(s) with Max Working Hours:";
    for (auto x : maxs)
        cout << ' ' << x;
    cout << endl
         << "Day(s) with Min Working Hours:";
    for (auto x : mins)
        cout << ' ' << x;
    cout << endl;
}

void HandleCommand ::update_team_bonus(vector<string> commands)
{
    int teamId = stoi(commands[1]), bonus_percentage = stoi(commands[2]);
    bool is_team_found = false;
    if (bonus_percentage < 1 || bonus_percentage > 100)
    {
        cout << ERROR_ARGUMENTS << endl;
        return;
    }
    for (int i = 0; i < teams.size(); i++)
        if (stoi(teams[i].get_team_id()) == teamId)
        {
            teams[i].update_bonus(bonus_percentage);
            is_team_found = true;
        }
    if (!is_team_found)
        cout << ERROR_TEAM << endl;
}

void HandleCommand ::report_employee_per_hour(vector<string> commands)
{
    int start = stoi(commands[1]), end = stoi(commands[2]);
    vector<pair<double, int>> avr_time;
    for (int i = start; i < end; i++)
    {
        double count = 0;
        int begin = i, last = i + 1;
        for (auto x : working_hours)
        {
            if ((begin >= x->get_pair().first && begin < x->get_pair().second) || (last > x->get_pair().first &&
                                                                                   last <= x->get_pair().second))
                count++;
        }
        count /= 30;
        avr_time.push_back({count, i});
        cout << begin << '-' << last << ": " << count << endl;
    }
    cout << "---" << endl;
    vector<double> maxs, mins;
    bool is_max = false, is_min = false;
    for (int i = 0; i < avr_time.size(); i++)
    {
        for (int j = 0; j < avr_time.size(); j++)
        {
            if (i != j && avr_time[i].first > avr_time[j].first)
                is_max = true;
            if (i != j && avr_time[i].first < avr_time[j].first)
                is_min = true;
        }
        if (is_max && !is_min)
            maxs.push_back(avr_time[i].second);
        if (!is_max && is_min)
            mins.push_back(avr_time[i].second);
    }
    cout << "Period(s) with Max Working Employees:";
    for (auto x : maxs)
        cout << ' ' << x << '-' << x + 1;
    cout << endl
         << "Period(s) with Min Working Employees:";
    for (auto x : mins)
        cout << ' ' << x << '-' << x + 1;
    cout << endl;
}

void HandleCommand ::free_trash_data(int day, int id)
{
    for (int i = 0; i < working_hours.size(); i++)
    {
        if (id == working_hours[i]->get_id() && day == working_hours[i]->get_day())
        {
            delete working_hours[i];
            working_hours.erase(working_hours.begin() + i);
            i--;
        }
    }
}

void HandleCommand ::delete_working_hours(vector<string> command_words)
{
    int day = stoi(command_words[2]);
    int employee_index = find_employee(stoi(command_words[1]));
    if (employee_index == NOT_FOUND)
        cout << ERROR_EMPLOYEE << endl;
    else
    {
        if (day > 30 || day < 1)
            cout << ERROR_ARGUMENTS << endl;
        else
        {
            employees[employee_index].delete_working_hours(day);
            free_trash_data(day, stoi(command_words[1]));
            cout << DONE_SUCCESSFULLY_MESSAGE << endl;
        }
    }
}

void HandleCommand ::report_employee_salary(vector<string> command_words)
{
    int employee_index = find_employee(stoi(command_words[1]));
    if (employee_index == NOT_FOUND)
        cout << ERROR_EMPLOYEE << endl;
    else
    {
        employees[employee_index].show_personal_info();
        employees[employee_index].show_working_info();
    }
}

void HandleCommand ::get_command()
{
    string command, line;
    while (getline(cin, line))
    {
        // for (auto i : employees)
        //     i.print_file();
        // for (auto i : teams)
        //     i.print_team();
        vector<string> command_words = split_words(line, SPACE);
        command = command_words[0];
        if (command == "show_salary_config")
        {
            show_salary_config(command_words[1]);
        }
        if (command == "add_working_hours")
        {
            add_working_hour(command_words);
        }
        if (command == "update_salary_config")
            update_salary_config(command_words);
        if (command == "report_total_hours_per_day")
            report_total_hours_per_day(command_words);
        if (command == "report_employee_per_hour")
            report_employee_per_hour(command_words); // needs a fix
        if (command == "update_team_bonus")
            update_team_bonus(command_words);
        if (command == "delete_working_hours")
            delete_working_hours(command_words);
        if (command == "report_employee_salary")
            report_employee_salary(command_words);
    }
}

void HandleCommand ::match_employees_and_teams()
{
    vector<int> mem_ids;
    int employee_index;
    for (int i = 0; i < teams.size(); i++)
    {
        mem_ids = teams[i].get_mem_ids();
        employee_index = find_employee(teams[i].get_head_id());
        employees[employee_index].match_to_team(&teams[i]);
        for (int j = 0; j < mem_ids.size(); j++)
        {
            employee_index = find_employee(mem_ids[j]);
            employees[employee_index].match_to_team(&teams[i]);
        }
    }
}

void HandleCommand ::match_employees_and_levels()
{
    for (int i = 0; i < employees.size(); i++)
    {
        string level_name = employees[i].get_level_name();
        int level_index = find_level(level_name);
        if (level_index != NOT_FOUND)
        {
            employees[i].set_level(&salaries[level_index]);
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
    match_employees_and_levels();
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