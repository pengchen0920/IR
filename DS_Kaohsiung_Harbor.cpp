#include <iostream>     //input, output
#include <string>
#include <sstream>        //read file
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;
 
//forward declaration
class Ship;
class ShipList;
class Task;
class TaskList;
 
 
 
//Time struct
struct Time
{
    int hour;
    int min;
};
 
//parse HH:MM string time to integer minutes formate
Time parseTime(string sTime){
    string delimiter = ":";
     
    size_t pos = 0;
    string token;
    pos = sTime.find(delimiter);
    token = sTime.substr(0, pos);
    int hour  = stoi(token);
    sTime.erase(0, pos + delimiter.length());
    int minute = stoi(sTime);
     
    return {hour, minute};
}
 
//parse integer minutes formate time to HH:MM string time
string parseTime(int time)
{
    string hour = ((time/60) <10) ? ("0"+to_string(time/60)) : to_string(time/60);
    string minute = ((time%60) <10) ? ("0"+to_string(time%60)) : to_string(time%60);
    return hour + ":" + minute;
}
 
class Ship
{
private:
    string shipName;
    int shipWeight;
    string countryName;
    string captainName;
    bool isLoadedDanger;
public:
    //constructor
    Ship(string sName, int sWeight, string country, string cName, bool isDanger);
    //get ship information
    string getShipName(){return shipName;};
    int getShipWeight(){return shipWeight;};
    string getCountryName(){return countryName;};
    string getCaptainName(){return captainName;};
    bool getIsLoadedDanger(){return isLoadedDanger;};
    //print
    void printShipInfo();
};
 
 
Ship::Ship(string sName, int sWeight, string country, string cName, bool isDanger)
{
    shipName = sName;
    shipWeight = sWeight;
    countryName = country;
    captainName = cName;
    isLoadedDanger = isDanger;
}
 
 
void Ship::printShipInfo()
{
    cout << shipName << "\t" << shipWeight << "\t" << countryName << "\t" << captainName << "\t" << isLoadedDanger << endl;
}
 
class ShipList
{
private:
    int shipCapacity;
    int shipNum;
    Ship **shipList;
public:
    ShipList(int numOfShips);
    ~ShipList();
    void addShip(string shipName, int shipWeight, string countryName, string captainName, bool isLoadedDanger);
    Ship* getShipByShipName(string shipName);
    void printShips();
};
 
//constructor
ShipList::ShipList(int numOfShips)
{
    shipNum = 0;
    shipCapacity = numOfShips;
    shipList = new Ship*[numOfShips];
    //initialize nullptr
    for(int i=0; i<shipCapacity; i++)
    {
        shipList[i] = nullptr;
    }
}
 
//destructor
ShipList::~ShipList()
{
    for(int i=0; i<shipNum; i++)
    {
        delete shipList[i];
        shipList[i] = nullptr;
    }
}
 
//print every ship's info in the list
void ShipList::printShips()
{
    cout << "***************************************" << endl;
    for(int i=0; i<shipNum; i++)
        shipList[i]->printShipInfo();
    cout << "***************************************" << endl;
}
 
//add new Ship into shipList
void ShipList::addShip(string shipName, int shipWeight, string countryName, string captainName, bool isLoadedDanger){
    if(shipNum<shipCapacity)
    {
        Ship* newShip = new Ship(shipName, shipWeight, countryName, captainName, isLoadedDanger);
        shipList[shipNum] = newShip;
        shipNum++;
    }
}
 
//search ship from shipList
Ship* ShipList::getShipByShipName(string shipName)
{
    for(int i=0; i<shipNum; i++)
    {
        if(shipList[i]->getShipName().compare(shipName) == 0)
        {
            return shipList[i];
        }
    }
    return nullptr;
}
 
class Task
{
protected:
    Ship* shipPtr;
    int startTime;
    string type;
    int port;
    int startPier;
    int destPier;
    int realStartTime;
public:
    Task();
    virtual ~Task();
    //get ship info
    string getShipName(){return shipPtr->getShipName();};
    int getShipWeight(){return shipPtr->getShipWeight();};
    bool getIsLoadedDanger(){return shipPtr->getIsLoadedDanger();};
    int getStartTime(){return startTime;};
    string getType(){return type;};
    int getPort(){return port;};
    void setStartPier(int newStartPier){startPier = newStartPier;};
    void setDestPier(int newDestPier){destPier = newDestPier;};
    int getStartPier(){return startPier;};
    int getDestPier(){return destPier;};
    virtual void printTaskInfo()=0;
    bool isNight();
    virtual int predictWorkTime(const double a[], const double b[], const double c[], const int t[], const int m);
};
 
//constructor
Task::Task()
{
     
}
 
Task::~Task(){
     
}
bool Task::isNight(){
    bool night = false;
    int time = this->getStartTime();
    if( (time>=1080 && time<=1439) || (time>=0 && time<=360))
        night = true;
    return night;
}
int Task::predictWorkTime(const double a[], const double b[], const double c[], const int t[], const int m){
    int workTime=0;
    return workTime;
}
 
class InTask : public Task
{
public:
    InTask(Ship* sPtr, string sTime, string aType, int param1, int param2);
    ~InTask();
    void printTaskInfo();
    int movingDST(int m,const int t[]);
    int predictWorkTime(const double a[], const double b[], const double c[], const int t[], const int m);
};
 
InTask::InTask(Ship* sPtr, string sTime, string aType, int param1, int param2){
    Time parsedTime = parseTime(sTime);
    shipPtr = sPtr;
    startTime = (parsedTime.hour*60) + parsedTime.min;
    type = aType;
    port = param1;
    startPier = 0;
    destPier = param2;
    realStartTime = 0;
}
 
InTask::~InTask(){
    shipPtr = nullptr;
}
 
void InTask::printTaskInfo(){
    cout << "IN!" << endl;
}
 
int InTask::movingDST(int m, const int *t){
    int distance = 0;
    int start_row = 0;
    int dest_row = 0;
    //reset startPier and destPier in oder to calculate movingDST
    //t[1]: InTask must have t[1]
    distance += t[1];
    if(this->getPort()==1){
        this->setStartPier(-1);
    }
    else{
        this->setStartPier(m+1);
    };
     
    //t[3]: if there is a odd pier and even pier, there is t[3]
    if((abs(this->getStartPier())%2 + abs(this->getDestPier())%2)==1)
        distance+=t[3];
     
    //t[2]: how many rows are there between two piers
    start_row = ceil(static_cast<double>(this->getStartPier())/2);
    dest_row = ceil(static_cast<double>(this->getDestPier())/2);
    distance += abs(dest_row-start_row)*t[2];
     
    return distance;
}
 
int InTask::predictWorkTime(const double *a, const double *b, const double *c, const int *t, const int m){
    double workTime = 0;
     
    workTime = a[0] + a[1]*(this->movingDST(m,t)) + a[2]*(this->getShipWeight()) + a[3]*(this->getIsLoadedDanger()?1:0) +a[4]*(this->isNight()?1:0);
    return workTime;
}
 
 
class OutTask : public Task
{
public:
    OutTask(Ship* sPtr, string sTime, string aType, int param1, int param2);
    ~OutTask();
    void printTaskInfo();
    int movingDST(int m,const int t[]);
    int predictWorkTime(const double a[], const double b[], const double c[], const int t[], const int m);
};
OutTask::OutTask(Ship* sPtr, string sTime, string aType, int param1, int param2){
    Time parsedTime = parseTime(sTime);
    shipPtr = sPtr;
    startTime = (parsedTime.hour*60) + parsedTime.min;
    type = aType;
    port = param1;
    startPier = param2;
    destPier = 0;
    realStartTime = 0;
}
OutTask::~OutTask(){
    shipPtr = nullptr;
}
void OutTask::printTaskInfo(){
    cout << "OUT!" << endl;
}
 
int OutTask::movingDST(int m, const int *t){
    int distance = 0;
    int start_row = 0;
    int dest_row = 0;
    //reset startPier and destPier in oder to calculate movingDST
    //t[1]: OutTask must have t[1]
    distance += t[1];
    if(this->getPort()==1){
        this->setDestPier(-1);
    }
    else{
        this->setDestPier(m+1);
    };
     
    //t[3]: if there is a odd pier and even pier, there is t[3]
    if((abs(this->getStartPier())%2 + abs(this->getDestPier())%2)==1)
        distance+=t[3];
     
    //t[2]: how many rows are there between two piers
    start_row = ceil(static_cast<double>(this->getStartPier())/2);
    dest_row = ceil(static_cast<double>(this->getDestPier())/2);
    distance += abs(dest_row-start_row)*t[2];
     
    return distance;
}
 
int OutTask::predictWorkTime(const double *a, const double *b, const double *c, const int *t, const int m){
    double workTime = 0;
    workTime = b[0] + b[1]*(this->movingDST(m,t)) + b[2]*(this->getShipWeight()) + b[3]*(this->getIsLoadedDanger()?1:0) +b[4]*(this->isNight()?1:0);
    return workTime;
}
 
class TranTask : public Task
{
public:
    TranTask(Ship* sPtr, string sTime, string aType, int param1, int param2);
    ~TranTask();
    void printTaskInfo();
    int movingDST(int m,const int t[]);
    int predictWorkTime(const double a[], const double b[], const double c[], const int t[], const int m);
};
TranTask::TranTask(Ship* sPtr, string sTime, string aType, int param1, int param2){
    Time parsedTime = parseTime(sTime);
    shipPtr = sPtr;
    startTime = (parsedTime.hour*60) + parsedTime.min;
    type = aType;
    port = 0;
    startPier = param1;
    destPier = param2;
    realStartTime = 0;
}
 
TranTask::~TranTask(){
    shipPtr = nullptr;
 
}
 
void TranTask::printTaskInfo(){
    cout << "TRAN!" << endl;
}
int TranTask::movingDST(int m, const int *t){
    int distance = 0;
    int start_row = 0;
    int dest_row = 0;
    //reset startPier and destPier in oder to calculate movingDST
    //t[1]: NO!
 
    //t[3]: if there is a odd pier and even pier, there is t[3]
    if((abs(this->getStartPier())%2 + abs(this->getDestPier())%2)==1)
        distance+=t[3];
     
    //t[2]: how many rows are there between two piers
    start_row = ceil(static_cast<double>(this->getStartPier())/2);
    dest_row = ceil(static_cast<double>(this->getDestPier())/2);
    distance += abs(dest_row-start_row)*t[2];
     
    return distance;
}
 
int TranTask::predictWorkTime(const double *a, const double *b, const double *c, const int *t, const int m){
    double workTime = 0;
    workTime = c[0] + c[1]*(this->movingDST(m,t)) + c[2]*(this->getShipWeight()) + c[3]*(this->getIsLoadedDanger()?1:0) +c[4]*(this->isNight()?1:0);
    return workTime;
}
 
class TaskList
{
private:
    //array information
    vector<Task*> taskList;
     
public:
    TaskList();
    ~TaskList();
    //void addTask()
    vector<Task*> getTaskList(){return taskList;};
    void addInTask(Ship* shipPtr, string sTime, string type, int param1, int param2);
    void addOutTask(Ship* shipPtr, string sTime, string type, int param1, int param2);
    void addTranTask(Ship* shipPtr, string sTime, string type, int param1, int param2);
    void printTasks();
    int movingDST(int m, int t[]);
    bool isNight();
    int predictWorkTime(const double a[],const double b[],const double c[]);
     
};
 
//constructor: create array of task -> do nothing ,because of vector
TaskList::TaskList()
{
 
}
//destructor: delete task in taskList
TaskList::~TaskList()
{
    while(taskList.size()>0)
    {
        delete this->taskList.back();
        this->taskList.pop_back();
    }
}
 
//print each task's infomation in taskList
void TaskList::printTasks()
{
    cout << "***********************************************" << endl;
    for(int i=0; i<taskList.size(); i++){
        taskList[i]->printTaskInfo();
    }
    cout << "***********************************************" << endl;
}
 
//add a InTask into taskList
void TaskList::addInTask(Ship* shipPtr, string sTime, string type, int param1, int param2)
{
        Task* newTask = new InTask(shipPtr, sTime,type, param1, param2);
        this->taskList.push_back(newTask);
}
//add a OutTask into taskList
void TaskList::addOutTask(Ship* shipPtr, string sTime, string type, int param1, int param2)
{
    Task* newTask = new OutTask(shipPtr, sTime,type, param1, param2);
    this->taskList.push_back(newTask);
}
//add a TranTask into taskList
void TaskList::addTranTask(Ship* shipPtr, string sTime, string type, int param1, int param2)
{
    Task* newTask = new TranTask(shipPtr, sTime,type, param1, param2);
    this->taskList.push_back(newTask);
}
 
 
 
 
int main(){
     
    string input_line;
    int m =0;
    double a[5];
    double b[5];
    double c[5];
    int t[4]; t[0]=0;  //t1==t[1]; t2==t[2]; t3==t[3]
    string shipFileName;
    string taskFileName;
 
    cin >> input_line;
    cin >> shipFileName >> taskFileName;
    size_t pos = 0;
    string delimiter = ",";
    pos = input_line.find(delimiter);
    m = stoi(input_line.substr(0, pos));
    input_line.erase(0, pos + delimiter.length());
 
    for(int i=0 ; i<5; i++){
        pos = input_line.find(delimiter);
        a[i] = stod(input_line.substr(0, pos));
        input_line.erase(0, pos + delimiter.length());
    }
    for(int i=0 ; i<5; i++){
        pos = input_line.find(delimiter);
        b[i] = stod(input_line.substr(0, pos));
        input_line.erase(0, pos + delimiter.length());
    }
    for(int i=0 ; i<5; i++){
        pos = input_line.find(delimiter);
        c[i] = stod(input_line.substr(0, pos));
        input_line.erase(0, pos + delimiter.length());
    }
    for(int i=1 ; i<4; i++){
        pos = input_line.find(delimiter);
        t[i] = stoi(input_line.substr(0, pos));
        input_line.erase(0, pos + delimiter.length());
    }
 
 
    //count ship / task numbers
    ifstream infile(shipFileName);
    int shipCount = count(istreambuf_iterator<char>(infile), istreambuf_iterator<char>(), '\n') +1;
    infile.close();
     
    //create class
    ShipList shipList(shipCount);
    TaskList taskList;
 
    string line;
    //read ship information
    infile.open(shipFileName);
    while(getline(infile, line))
    {
        string shipName;
        int shipWeight=0;
        string countryName;
        string captainName;
        bool isLoadedDanger=false;
 
        string delimiter = ",";
        size_t last = 0;
        size_t next = 0;
        //shipName
        next = line.find(delimiter, last);
        shipName = line.substr(last, next-last);
        last = next + 1;
        //shipWeight
        next = line.find(delimiter, last);
        shipWeight = stoi(line.substr(last, next-last));
        last = next + 1;
        //countryName
        next = line.find(delimiter, last);
        countryName = line.substr(last, next-last);
        last = next + 1;
        //captainName
        next = line.find(delimiter, last);
        captainName = line.substr(last, next-last);
        last = next + 1;
        //isLoadedDanger
        isLoadedDanger = (line.substr(last)=="Y") ? true : false;
 
        //create ship
        shipList.addShip(shipName, shipWeight, countryName, captainName, isLoadedDanger);
    }
    infile.close();
 
 
    //read task information
    vector<int> predictOutput;
    infile.open(taskFileName);
    int exception_count = 0;
    while(getline(infile, line))
    {
        //taskFile may not be complete
        //through exception to disregard the task input
        //count how many task input is incomplete
 
        string shipName;
        string startTime;
        string type;
        int param1 =0;
        int param2 =0;
 
        //string split
        string delimiter = ",";
        size_t pos = 0;
 
        //shipName
        pos = line.find(delimiter);
        shipName = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
 
        //startTime
        //if startTime wasn's there, line.substr(0,1) won't be able to use stoi->try catch
        try{
            pos = line.find(delimiter);
            int check_start_valid = stoi(line.substr(0, pos));
            startTime = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());
        }
        catch(invalid_argument start_time){
            exception_count++;
            predictOutput.push_back(-1);
            continue;
        }
 
        //type
        pos = line.find(delimiter);
        type = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
 
        //Because param1 & param2 use stoi->try catch
        //param1
        try{
            pos = line.find(delimiter);
            param1 = stoi(line.substr(0, pos));
            line.erase(0, pos + delimiter.length());
            //param2
            param2 = stoi(line);
        }
        catch(invalid_argument params){
            exception_count++;
            predictOutput.push_back(-1);
            continue;
        }
        Ship* shipPtr = shipList.getShipByShipName(shipName);
        //if you lost (1)startTime (2)param1 (3)param2 then the task object won't be added in the taskList, exception++
        //NO shipname entry -> task's shipPtr == NULL
        //NO type entry -> then it won't have a type, it won't be created as an object
        if(type=="I"){
            taskList.addInTask(shipPtr, startTime, type, param1, param2);
            //this is the MovingDST:taskList.movingDST(m,t)
            //predictOutput.push_back(taskList.movingDST(m,t));
            predictOutput.push_back(-100);
            //-100 as a MARK to use later to apply predictWorkTime()
            continue;
        }
        else if (type=="O"){
            taskList.addOutTask(shipPtr, startTime, type, param1, param2);
            //this is the MovingDST:taskList.movingDST(m,t)
            predictOutput.push_back(-100);
            continue;
        }
        else if (type=="T"){
            taskList.addTranTask(shipPtr, startTime, type, param1, param2);
            //this is the MovingDST:taskList.movingDST(m,t)
            predictOutput.push_back(-100);
            continue;
        }
        //if it doesn't has type, it will goes to this block
        //and there would be one more wrong:exception_count++
        else{
            predictOutput.push_back(-1);
            exception_count++;
            continue;
        }
         
    }
    infile.close();
    int taskList_pos = 0;
    for(int i = 0; i<predictOutput.size(); i++){
        if(predictOutput[i]==(-100)){
            predictOutput[i] = taskList.getTaskList()[taskList_pos]->predictWorkTime(a, b, c, t, m);
            taskList_pos++;
            }
    }
    for(int i = 0; i<predictOutput.size(); i++){
        if(i<predictOutput.size()-1)
            cout << predictOutput[i] << ",";
        else
            cout << predictOutput[i] << endl;
    }
    cout << exception_count;
    return 0;
}