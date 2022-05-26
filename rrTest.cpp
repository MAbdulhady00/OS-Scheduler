// this code was borrowed from https://shivammitra.com/operating%20system/roundrobin-scheduling-program/# for testing purposes

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstring>
using namespace std;

struct process
{
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};
vector<int> finished;

bool compare1(process p1, process p2)
{
    if (p1.arrival_time == p2.arrival_time)
        return p1.pid < p2.pid;
    return p1.arrival_time < p2.arrival_time;
}

bool compare2(process p1, process p2)
{
    return p1.pid < p2.pid;
}

int main(int argc, char **argv)
{

    FILE *logFile = fopen("out.log", "w+");
    if (logFile == NULL)
    {
        printf("Error!");
        exit(1);
    }

    int n;
    int tq;
    struct process p[1000];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[1000];
    int idx;

    cout << setprecision(2) << fixed;

    tq = atoi(argv[1]);

    {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        FILE *file = fopen("processes.txt", "r");
        if (NULL == file)
        {
            perror("file can't be opened \n");
        }
        int i = 0;
        while ((read = getline(&line, &len, file)) != -1)
        {
            if (line[0] == '#')
                continue;
            p[i].pid = atoi(strtok(line, "\t"));
            p[i].arrival_time = atoi(strtok(NULL, "\t"));
            p[i].burst_time = atoi(strtok(NULL, "\t"));
            atoi(strtok(NULL, "\t"));
            burst_remaining[i] = p[i].burst_time;
            i++;
        }
        if (line)
            free(line);
        n = i;
    }
    sort(p, p + n, compare1);

    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[1000];
    memset(mark, 0, sizeof(mark));
    mark[0] = 1;

    while (completed != n)
    {
        idx = q.front();
        q.pop();

        if (burst_remaining[idx] == p[idx].burst_time)
        {
            p[idx].start_time = max(current_time, p[idx].arrival_time);
            total_idle_time += p[idx].start_time - current_time;
            current_time = p[idx].start_time;
        }

        if (burst_remaining[idx] - tq > 0)
        {
            burst_remaining[idx] -= tq;
            current_time += tq;
        }
        else
        {
            finished.push_back(idx);
            current_time += burst_remaining[idx];
            burst_remaining[idx] = 0;
            completed++;

            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

            total_turnaround_time += p[idx].turnaround_time;
            total_waiting_time += p[idx].waiting_time;
            total_response_time += p[idx].response_time;
        }

        for (int i = 1; i < n; i++)
        {
            if (burst_remaining[i] > 0 && p[i].arrival_time <= current_time && mark[i] == 0)
            {
                q.push(i);
                mark[i] = 1;
            }
        }
        if (burst_remaining[idx] > 0)
        {
            q.push(idx);
        }

        if (q.empty())
        {
            for (int i = 1; i < n; i++)
            {
                if (burst_remaining[i] > 0)
                {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }

    avg_turnaround_time = (float)total_turnaround_time / n;
    avg_waiting_time = (float)total_waiting_time / n;
    avg_response_time = (float)total_response_time / n;
    cpu_utilisation = ((p[n - 1].completion_time - total_idle_time) / (float)p[n - 1].completion_time) * 100;
    throughput = float(n) / (p[n - 1].completion_time - p[0].arrival_time);

    //sort(p, p + n, compare2);

    // cout<<endl;
    // cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<"\n"<<endl;

    // for(int i = 0; i < n; i++) {
    //     cout<<p[i].pid<<"\t"<<p[i].arrival_time<<"\t"<<p[i].burst_time<<"\t"<<p[i].start_time<<"\t"<<p[i].completion_time<<"\t"<<p[i].turnaround_time<<"\t"<<p[i].waiting_time<<"\t"<<p[i].response_time<<"\t"<<"\n"<<endl;
    // }
    fprintf(logFile, "#At time x process y state arr w total z remain y wait k\n");
    for (int i = 0; i < finished.size(); i++)
    {
        double WTA = ((double)p[finished[i]].completion_time - (double)p[finished[i]].arrival_time) / p[finished[i]].burst_time;
        WTA = (int)(WTA * 100 + .5) / 100.0;
        fprintf(logFile, "At time %d process %d ", p[finished[i]].completion_time, p[finished[i]].pid);
        fprintf(logFile, "finished arr %d total %d remain 0 wait %d TA %d WTA %.2lf\n", p[finished[i]].arrival_time, p[finished[i]].burst_time, p[finished[i]].waiting_time, p[finished[i]].turnaround_time, WTA);
    }
    // cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    // cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    // cout<<"Average Response Time = "<<avg_response_time<<endl;
    // cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    // cout<<"Throughput = "<<throughput<<" process/unit time"<<endl;
}
