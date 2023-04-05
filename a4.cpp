#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

const string LEVEL[] = {"team-lead", "senior", "expert", "junior"};
class Team
{
public:
    void set_bonus();
    void update_bonus();

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
    int calculate_duration();

private:
    int employee_id;
    int day;
    vector<pair<int, int>> working_hour;
};
class Salary
{
public:
    void update_level();
    void update_base_salary();
    void update_salary_per_hour();
    void update_salary_per_extra_hour();
    void update_official_working_hours();
    void update_tax_percantage();
    string get_level();
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
    int calculate_salary();
    int calculate_working_hours();
    int calculate_tax();
    int calculate_bonus();
    int calculate_total_earning();
    void add_working_hour();
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
int main()
{
    return 0;
}