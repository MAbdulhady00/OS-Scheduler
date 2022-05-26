#include <iostream>
#include <vector>
#include <string.h>
using namespace std;
void SerailizeInput()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *file = fopen("processes.txt", "r");
    if (NULL == file)
    {
        perror("file can't be opened \n");
    }
    vector<int> aTime;
    vector<int> rTime;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[0] == '#')
            continue;
        atoi(strtok(line, "\t"));
        aTime.push_back(atoi(strtok(NULL, "\t")));
        rTime.push_back(atoi(strtok(NULL, "\t")));
        atoi(strtok(NULL, "\t"));
    }
    if (line)
        free(line);
    for (int i = 0; i < aTime.size(); i++)
    {
        cout << aTime[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < rTime.size(); i++)
    {
        cout << rTime[i] << " ";
    }
    cout << endl;
}
int main()
{
    SerailizeInput();
}
