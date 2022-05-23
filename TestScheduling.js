"use strict";
var __assign = (this && this.__assign) || function () {
    __assign = Object.assign || function(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
                t[p] = s[p];
        }
        return t;
    };
    return __assign.apply(this, arguments);
};
var __spreadArray = (this && this.__spreadArray) || function (to, from, pack) {
    if (pack || arguments.length === 2) for (var i = 0, l = from.length, ar; i < l; i++) {
        if (ar || !(i in from)) {
            if (!ar) ar = Array.prototype.slice.call(from, 0, i);
            ar[i] = from[i];
        }
    }
    return to.concat(ar || Array.prototype.slice.call(from));
};
exports.__esModule = true;
function safeDiv(a, b) {
    var div = a / b;
    if (div === Infinity)
        return "inf";
    return div.toFixed(2);
}
function rr(processId, arrivalTime, burstTime, timeQuantum) {
    var processesInfo = processId
        .map(function (item, index) {
        return {
            job: item,
            at: arrivalTime[index],
            bt: burstTime[index]
        };
    })
        .sort(function (obj1, obj2) {
        if (obj1.at > obj2.at)
            return 1;
        if (obj1.at < obj2.at)
            return -1;
        return 0;
    });
    var solvedProcessesInfo = [];
    //const ganttChartInfo: ganttChartInfoType = [];
    var readyQueue = [];
    var currentTime = processesInfo[0].at;
    var unfinishedJobs = __spreadArray([], processesInfo, true);
    var remainingTime = processesInfo.reduce(function (acc, process) {
        acc[process.job] = process.bt;
        return acc;
    }, {});
    readyQueue.push(unfinishedJobs[0]);
    var _loop_1 = function () {
        if (readyQueue.length === 0 && unfinishedJobs.length > 0) {
            // Previously idle
            readyQueue.push(unfinishedJobs[0]);
            currentTime = readyQueue[0].at;
        }
        var processToExecute = readyQueue[0];
        if (remainingTime[processToExecute.job] <= timeQuantum) {
            // Burst time less than or equal to time quantum, execute until finished
            var remainingT = remainingTime[processToExecute.job];
            remainingTime[processToExecute.job] -= remainingT;
            var prevCurrentTime = currentTime;
            currentTime += remainingT;
            //ganttChartInfo.push({
            //  job: processToExecute.job,
            //  start: prevCurrentTime,
            //  stop: currentTime,
            //});
        }
        else {
            remainingTime[processToExecute.job] -= timeQuantum;
            var prevCurrentTime = currentTime;
            currentTime += timeQuantum;
            //ganttChartInfo.push({
            //  job: processToExecute.job,
            //  start: prevCurrentTime,
            //  stop: currentTime,
            //});
        }
        var processToArriveInThisCycle = processesInfo.filter(function (p) {
            return (p.at <= currentTime &&
                p !== processToExecute &&
                !readyQueue.includes(p) &&
                unfinishedJobs.includes(p));
        });
        // Push new processes to readyQueue
        readyQueue.push.apply(readyQueue, processToArriveInThisCycle);
        // Requeueing (move head/first item to tail/last)
        readyQueue.push(readyQueue.shift());
        // When the process finished executing
        if (remainingTime[processToExecute.job] === 0) {
            var indexToRemoveUJ = unfinishedJobs.indexOf(processToExecute);
            if (indexToRemoveUJ > -1) {
                unfinishedJobs.splice(indexToRemoveUJ, 1);
            }
            var indexToRemoveRQ = readyQueue.indexOf(processToExecute);
            if (indexToRemoveRQ > -1) {
                readyQueue.splice(indexToRemoveRQ, 1);
            }
            solvedProcessesInfo.push(__assign(__assign({}, processToExecute), { ft: currentTime, tat: currentTime - processToExecute.at, wat: currentTime - processToExecute.at - processToExecute.bt }));
        }
    };
    while (Object.values(remainingTime).reduce(function (acc, cur) {
        return acc + cur;
    }, 0) &&
        unfinishedJobs.length > 0) {
        _loop_1();
    }
    // Sort the processes arrival time and then by job name
    solvedProcessesInfo.sort(function (obj1, obj2) {
        if (obj1.at > obj2.at)
            return 1;
        if (obj1.at < obj2.at)
            return -1;
        if (obj1.job > obj2.job)
            return 1;
        if (obj1.job < obj2.job)
            return -1;
        return 0;
    });
    console.log(solvedProcessesInfo);
}
;
function npp(processId, arrivalTime, burstTime, priorities) {
    var processesInfo = processId
        .map(function (item, index) {
        return {
            job: item,
            at: arrivalTime[index],
            bt: burstTime[index],
            priority: priorities[index]
        };
    })
        .sort(function (process1, process2) {
        if (process1.at > process2.at)
            return 1;
        if (process1.at < process2.at)
            return -1;
        if (process1.priority > process2.priority)
            return 1;
        if (process1.priority < process2.priority)
            return -1;
        return 0;
    });
    var finishTime = [];
    //let ganttChartInfo: ganttChartInfoType = [];
    var solvedProcessesInfo = [];
    var readyQueue = [];
    var finishedJobs = [];
    var _loop_2 = function (i) {
        if (i === 0) {
            readyQueue.push(processesInfo[0]);
            finishTime.push(processesInfo[0].at + processesInfo[0].bt);
            console.log("At time ".concat(processesInfo[0].at, " process ").concat(processesInfo[0].job, " started arr ").concat(processesInfo[0].at, " total ").concat(processesInfo[0].bt, " remain ").concat(processesInfo[0].bt, " wait 0"));
            console.log("At time ".concat(finishTime[0], " process ").concat(processesInfo[0].job, " finished arr ").concat(processesInfo[0].at, " total ").concat(processesInfo[0].bt, " remain 0 wait ").concat(finishTime[0] - processesInfo[0].at - processesInfo[0].bt, " TA ").concat(finishTime[0] - processesInfo[0].at, " WTA ").concat(safeDiv((finishTime[0] - processesInfo[0].at), (processesInfo[0].bt))));
            solvedProcessesInfo.push(__assign(__assign({}, processesInfo[0]), { ft: finishTime[0], tat: finishTime[0] - processesInfo[0].at, wat: finishTime[0] - processesInfo[0].at - processesInfo[0].bt }));
            processesInfo.forEach(function (p) {
                if (p.at <= finishTime[0] && !readyQueue.includes(p)) {
                    readyQueue.push(p);
                }
            });
            readyQueue.shift();
            finishedJobs.push(processesInfo[0]);
            //ganttChartInfo.push({
            //  job: processesInfo[0].job,
            //  start: processesInfo[0].at,
            //  stop: finishTime[0],
            //});
        }
        else {
            if (readyQueue.length === 0 &&
                finishedJobs.length !== processesInfo.length) {
                var unfinishedJobs = processesInfo
                    .filter(function (p) {
                    return !finishedJobs.includes(p);
                })
                    .sort(function (a, b) {
                    if (a.at > b.at)
                        return 1;
                    if (a.at < b.at)
                        return -1;
                    if (a.priority > b.priority)
                        return 1;
                    if (a.priority < a.priority)
                        return -1;
                    return 0;
                });
                readyQueue.push(unfinishedJobs[0]);
            }
            // Equal-priority processes are scheduled in FCFS order.
            var rqSortedByPriority = __spreadArray([], readyQueue, true).sort(function (a, b) {
                if (a.priority > b.priority)
                    return 1;
                if (a.priority < b.priority)
                    return -1;
                if (a.at > b.at)
                    return 1;
                if (a.at < b.at)
                    return -1;
                return 0;
            });
            var processToExecute = rqSortedByPriority[0];
            var previousFinishTime = finishTime[finishTime.length - 1];
            if (processToExecute.at > previousFinishTime) {
                finishTime.push(processToExecute.at + processToExecute.bt);
                var newestFinishTime_1 = finishTime[finishTime.length - 1];
                console.log("At time ".concat(processToExecute.at, " process ").concat(processToExecute.job, " started arr ").concat(processToExecute.at, " total ").concat(processToExecute.bt, " remain ").concat(processToExecute.bt, " wait 0"));
                //ganttChartInfo.push({
                //  job: processToExecute.job,
                //  start: processToExecute.at,
                //  stop: newestFinishTime,
                //});
            }
            else {
                finishTime.push(previousFinishTime + processToExecute.bt);
                var newestFinishTime_2 = finishTime[finishTime.length - 1];
                console.log("At time ".concat(previousFinishTime, " process ").concat(processToExecute.job, " started arr ").concat(processToExecute.at, " total ").concat(processToExecute.bt, " remain ").concat(processToExecute.bt, " wait ").concat(newestFinishTime_2 - processToExecute.at - processToExecute.bt));
                //ganttChartInfo.push({
                //  job: processToExecute.job,
                //  start: previousFinishTime,
                //  stop: newestFinishTime,
                //});
            }
            var newestFinishTime_3 = finishTime[finishTime.length - 1];
            console.log("At time ".concat(newestFinishTime_3, " process ").concat(processToExecute.job, " finished arr ").concat(processToExecute.at, " total ").concat(processToExecute.bt, " remain 0 wait ").concat(newestFinishTime_3 - processToExecute.at - processToExecute.bt, " TA ").concat(newestFinishTime_3 - processToExecute.at, " WTA ").concat(safeDiv((newestFinishTime_3 - processToExecute.at), (processToExecute.bt))));
            solvedProcessesInfo.push(__assign(__assign({}, processToExecute), { ft: newestFinishTime_3, tat: newestFinishTime_3 - processToExecute.at, wat: newestFinishTime_3 - processToExecute.at - processToExecute.bt }));
            processesInfo.forEach(function (p) {
                if (p.at <= newestFinishTime_3 &&
                    !readyQueue.includes(p) &&
                    !finishedJobs.includes(p)) {
                    readyQueue.push(p);
                }
            });
            var indexToRemove = readyQueue.indexOf(processToExecute);
            if (indexToRemove > -1) {
                readyQueue.splice(indexToRemove, 1);
            }
            finishedJobs.push(processToExecute);
        }
    };
    for (var i = 0; i < processesInfo.length; i++) {
        _loop_2(i);
    }
    // Sort the processes by job name within arrival time
    solvedProcessesInfo.sort(function (obj1, obj2) {
        if (obj1.at > obj2.at)
            return 1;
        if (obj1.at < obj2.at)
            return -1;
        if (obj1.job > obj2.job)
            return 1;
        if (obj1.job < obj2.job)
            return -1;
        return 0;
    });
    //console.log(solvedProcessesInfo);
}
;
var fs_1 = require("fs");
function isBlank(str) {
    return (!str || /^\s*$/.test(str));
}
var file = (0, fs_1.readFileSync)('./processes.txt', 'utf-8');
var lines = file.toString().replace(/\r\n/g, '\n').split('\n');
var processId = [];
var arrivalTime = [], burstTime = [], priority = [];
for (var _i = 0, lines_1 = lines; _i < lines_1.length; _i++) {
    var line = lines_1[_i];
    if (line.startsWith('#') || isBlank(line))
        continue;
    var split = line.trim().split(/\s+/);
    processId.push(Number(split[0]));
    arrivalTime.push(Number(split[1]));
    burstTime.push(Number(split[2]));
    priority.push(Number(split[3]));
}
console.log("#At time x process y state arr w total z remain y wait k");
npp(processId, arrivalTime, burstTime, priority);
