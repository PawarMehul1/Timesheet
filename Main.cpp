#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include <fstream>
#include <set>
#include <string>

using namespace std;

set<string> employees;
int strtoint(string str)
{
    int ans=0;
    for(int i=0; i<str.size(); i++)
    {
        ans*=10;
        ans+=(int)(str[i])-48;
    }
    return ans;
}
int timetosec(string time)
{
    int second=0;
    second+=(int)(time[7])-48;
    second+=((int)(time[6])-48)*10;

    second+=((int)(time[4])-48)*60;
    second+=((int)(time[3])-48)*600;

    second+=((int)(time[1])-48)*3600;
    second+=((int)(time[0])-48)*36000;

    return second;
}

string inttostring(int n)
{
    string ans="";
    while(n>0)
    {
        ans=(char)(48+(n%10))+ans;
        n/=10;
    }
    if(ans.length()==1)
        return "0"+ans;

    return ans;
}

vector<string> spilt(string str)
{
    vector<string> ans;
    int n=str.length();
    int index=0;

    while(index<n)
    {
        string tmp="";
        while(index<n && str[index]!=' ')
        {
            tmp+=str[index++];
        }
        index++;
        if(tmp!="")
            ans.push_back(tmp);
    }
    return ans;
}
int main()
{
    fstream file;
    file.open("input.txt", ios::in);

    string str;
    getline(file,str);

    vector<vector<string>> before;
    vector<vector<string>> after;

    int T=strtoint(str);
    
    for(int i=0; i<T; i++)
    {
        getline(file,str);
        vector<string> tockens=spilt(str);
        employees.insert(tockens[0]);

        vector<string> vals(4);
        if(tockens[3]=="clock-in")
        {
            vals[0]=tockens[0];
            vals[1]=tockens[1];
            vals[2]=tockens[2];
            vals[3]="null";

            before.push_back(vals);
        }
        else if(tockens[3]=="break-start")
        {
            for(int j=0; j<before.size(); j++)
            {
                if(tockens[0]==before[j][0] && tockens[1]==before[j][1])
                    before[j][3]=tockens[2];
            }
        }
        else if(tockens[3]=="break-stop")
        {
            vals[0]=tockens[0];
            vals[1]=tockens[1];
            vals[2]=tockens[2];
            vals[3]="null";

            after.push_back(vals);
        }
        else if(tockens[3]=="clock-out")
        {
            for(int j=0; j<after.size(); j++)
            {
                if(tockens[0]==after[j][0] && tockens[1]==after[j][1])
                    after[j][3]=tockens[2];
            }
        }
    }

    for(int j=0; j<after.size(); j++)
    {
        if(after[j][3]=="null")
        {
            for(int k=before.size()-1; k>=0; k--)
            {
                if(before[k][0]==after[j][0] && before[k][1]==after[j][1])
                {
                    int time=timetosec(before[k][3])-timetosec(before[k][2])-timetosec(after[j][2])+timetosec("19:30:00");
                    if(time<=21600)
                        after[j][3]="19:30:00";
                    else
                    {
                        after[j][2]="06:00:00";
                        before[k][3]=before[k][2];
                    }
                    break;
                }
            }
        }
    }

    file.close();

    int avg=0;
    int min=INT_MAX;
    int max=INT_MIN;

    for(auto id : employees)
    {
        int sum=0;
        for(int j=0; j<before.size(); j++)
        {
            if(before[j][0]==id)
                sum+=timetosec(before[j][3])-timetosec(before[j][2]);
            
        }
        for(int j=0; j<after.size(); j++)
        {
            if(after[j][0]==id && after[j][3]!="null")
                sum+=timetosec(after[j][3])-timetosec(after[j][2]);
            
            else if(after[j][0]==id && after[j][3]=="null")
                sum+=timetosec(after[j][2]);
        }
        if(sum>max)
            max=sum;
        if(sum<min)
            min=sum;
        avg+=sum;          
    }
    string ans="Case #1: ";

    ans+=inttostring(max/3600)+":"+inttostring((max%3600)/60)+":"+inttostring(max%60)+" ";
    ans+=inttostring(min/3600)+":"+inttostring((min%3600)/60)+":"+inttostring(min%60)+" ";
    avg/=employees.size();
    ans+=inttostring(avg/3600)+":"+inttostring((avg%3600)/60)+":"+inttostring(avg%60);
    
    ofstream OUTPUT("output.txt");
    OUTPUT<<ans<<endl;
    OUTPUT.close();

    return 0;
}