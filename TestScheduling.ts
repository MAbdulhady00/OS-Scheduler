var args = process.argv
function safeDiv(a : number, b : number) {
  var div = a / b;
  if(div === Infinity)
    return "inf";
  if(isNaN(div))
    return "-nan";
  
  return div.toFixed(2);
}

function rr(processId: number[], arrivalTime: number[], burstTime: number[], timeQuantum: number) {
  const processesInfo = processId
    .map((item, index) => {
      return {
        job: item,
        at: arrivalTime[index],
        bt: burstTime[index],
      };
    })
    .sort((obj1, obj2) => {
      if (obj1.at > obj2.at) return 1;
      if (obj1.at < obj2.at) return -1;
      return 0;
    });

  const solvedProcessesInfo = [];
  //const ganttChartInfo: ganttChartInfoType = [];

  const readyQueue: {
    job: number;
    at: number;
    bt: number;
  }[] = [];
  let currentTime = processesInfo[0].at;
  const unfinishedJobs = [...processesInfo];

  const remainingTime = processesInfo.reduce((acc, process) => {
    acc[process.job] = process.bt;
    return acc;
  }, {});

  readyQueue.push(unfinishedJobs[0]);
  while (
    Object.values(remainingTime).reduce((acc: number, cur: number) => {
      return acc + cur;
    }, 0) &&
    unfinishedJobs.length > 0
  ) {
    if (readyQueue.length === 0 && unfinishedJobs.length > 0) {
      // Previously idle
      readyQueue.push(unfinishedJobs[0]);
      currentTime = readyQueue[0].at;
    }

    const processToExecute = readyQueue[0];

    if (remainingTime[processToExecute.job] <= timeQuantum) {
      // Burst time less than or equal to time quantum, execute until finished
      const remainingT = remainingTime[processToExecute.job];
      remainingTime[processToExecute.job] -= remainingT;
      const prevCurrentTime = currentTime;
      currentTime += remainingT;
      
      //ganttChartInfo.push({
      //  job: processToExecute.job,
      //  start: prevCurrentTime,
      //  stop: currentTime,
      //});
    } else {
      remainingTime[processToExecute.job] -= timeQuantum;
      const prevCurrentTime = currentTime;
      currentTime += timeQuantum;

      //ganttChartInfo.push({
      //  job: processToExecute.job,
      //  start: prevCurrentTime,
      //  stop: currentTime,
      //});
    }
    const processToArriveInThisCycle = processesInfo.filter((p) => {
      return (
        p.at <= currentTime &&
        p !== processToExecute &&
        !readyQueue.includes(p) &&
        unfinishedJobs.includes(p)
      );
    });

    // Push new processes to readyQueue
    readyQueue.push(...processToArriveInThisCycle);

    // Requeueing (move head/first item to tail/last)
    readyQueue.push(readyQueue.shift());

    // When the process finished executing
    if (remainingTime[processToExecute.job] === 0) {
      const indexToRemoveUJ = unfinishedJobs.indexOf(processToExecute);
      if (indexToRemoveUJ > -1) {
        unfinishedJobs.splice(indexToRemoveUJ, 1);
      }
      const indexToRemoveRQ = readyQueue.indexOf(processToExecute);
      if (indexToRemoveRQ > -1) {
        readyQueue.splice(indexToRemoveRQ, 1);
      }

      solvedProcessesInfo.push({
        ...processToExecute,
        ft: currentTime,
        tat: currentTime - processToExecute.at,
        wat: currentTime - processToExecute.at - processToExecute.bt,
      });
      console.log(`At time ${currentTime} process ${processToExecute.job} finished arr ${processToExecute.at} total ${processToExecute.bt} remain 0 wait ${currentTime - processToExecute.at - processToExecute.bt} TA ${currentTime - processToExecute.at} WTA ${safeDiv((currentTime - processToExecute.at), (processToExecute.bt))}`)
    }
  }

  // Sort the processes arrival time and then by job name
  solvedProcessesInfo.sort((obj1, obj2) => {
    if (obj1.at > obj2.at) return 1;
    if (obj1.at < obj2.at) return -1;
    if (obj1.job > obj2.job) return 1;
    if (obj1.job < obj2.job) return -1;
    return 0;
  });

  return solvedProcessesInfo;
};

function npp(processId: number[], arrivalTime: number[], burstTime: number[], priorities: number[] ){
  const processesInfo = processId
    .map((item, index) => {
      return {
        job: item,
        at: arrivalTime[index],
        bt: burstTime[index],
        priority: priorities[index],
      };
    })
    .sort((process1, process2) => {
      if (process1.at > process2.at) return 1;
      if (process1.at < process2.at) return -1;
      if (process1.priority > process2.priority) return 1;
      if (process1.priority < process2.priority) return -1;
      return 0;
    });

  let finishTime: number[] = [];
  //let ganttChartInfo: ganttChartInfoType = [];

  const solvedProcessesInfo = [];
  const readyQueue = [];
  const finishedJobs = [];

  for (let i = 0; i < processesInfo.length; i++) {
    if (i === 0) {
      readyQueue.push(processesInfo[0]);
      finishTime.push(processesInfo[0].at + processesInfo[0].bt);
      console.log(`At time ${processesInfo[0].at} process ${processesInfo[0].job} started arr ${processesInfo[0].at} total ${processesInfo[0].bt} remain ${processesInfo[0].bt} wait 0`);
      console.log(`At time ${finishTime[0]} process ${processesInfo[0].job} finished arr ${processesInfo[0].at} total ${processesInfo[0].bt} remain 0 wait ${finishTime[0] - processesInfo[0].at - processesInfo[0].bt} TA ${finishTime[0] - processesInfo[0].at} WTA ${safeDiv((finishTime[0] - processesInfo[0].at), (processesInfo[0].bt))}`);
      solvedProcessesInfo.push({
        ...processesInfo[0],
        ft: finishTime[0],
        tat: finishTime[0] - processesInfo[0].at,
        wat: finishTime[0] - processesInfo[0].at - processesInfo[0].bt,
      });

      processesInfo.forEach((p) => {
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
    } else {
      if (
        readyQueue.length === 0 &&
        finishedJobs.length !== processesInfo.length
      ) {
        const unfinishedJobs = processesInfo
          .filter((p) => {
            return !finishedJobs.includes(p);
          })
          .sort((a, b) => {
            if (a.at > b.at) return 1;
            if (a.at < b.at) return -1;
            if (a.priority > b.priority) return 1;
            if (a.priority < a.priority) return -1;
            return 0;
          });
        readyQueue.push(unfinishedJobs[0]);
      }

      // Equal-priority processes are scheduled in FCFS order.
      const rqSortedByPriority = [...readyQueue].sort((a, b) => {
        if (a.priority > b.priority) return 1;
        if (a.priority < b.priority) return -1;
        if (a.at > b.at) return 1;
        if (a.at < b.at) return -1;
        return 0;
      });

      const processToExecute = rqSortedByPriority[0];

      const previousFinishTime = finishTime[finishTime.length - 1];

      if (processToExecute.at > previousFinishTime) {
        finishTime.push(processToExecute.at + processToExecute.bt);
        const newestFinishTime = finishTime[finishTime.length - 1];
        console.log(`At time ${processToExecute.at} process ${processToExecute.job} started arr ${processToExecute.at} total ${processToExecute.bt} remain ${processToExecute.bt} wait 0`);
        //ganttChartInfo.push({
        //  job: processToExecute.job,
        //  start: processToExecute.at,
        //  stop: newestFinishTime,
        //});
      } else {
        finishTime.push(previousFinishTime + processToExecute.bt);
        const newestFinishTime = finishTime[finishTime.length - 1];
        console.log(`At time ${previousFinishTime} process ${processToExecute.job} started arr ${processToExecute.at} total ${processToExecute.bt} remain ${processToExecute.bt} wait ${newestFinishTime - processToExecute.at - processToExecute.bt}`);
        //ganttChartInfo.push({
        //  job: processToExecute.job,
        //  start: previousFinishTime,
        //  stop: newestFinishTime,
        //});
      }

      const newestFinishTime = finishTime[finishTime.length - 1];
      console.log(`At time ${newestFinishTime} process ${processToExecute.job} finished arr ${processToExecute.at} total ${processToExecute.bt} remain 0 wait ${newestFinishTime - processToExecute.at - processToExecute.bt} TA ${newestFinishTime - processToExecute.at} WTA ${safeDiv((newestFinishTime - processToExecute.at), (processToExecute.bt))}`);
      solvedProcessesInfo.push({
        ...processToExecute,
        ft: newestFinishTime,
        tat: newestFinishTime - processToExecute.at,
        wat: newestFinishTime - processToExecute.at - processToExecute.bt,
      });

      processesInfo.forEach((p) => {
        if (
          p.at <= newestFinishTime &&
          !readyQueue.includes(p) &&
          !finishedJobs.includes(p)
        ) {
          readyQueue.push(p);
        }
      });

      const indexToRemove = readyQueue.indexOf(processToExecute);
      if (indexToRemove > -1) {
        readyQueue.splice(indexToRemove, 1);
      }

      finishedJobs.push(processToExecute);
    }
  }

  // Sort the processes by job name within arrival time
  solvedProcessesInfo.sort((obj1, obj2) => {
    if (obj1.at > obj2.at) return 1;
    if (obj1.at < obj2.at) return -1;
    if (obj1.job > obj2.job) return 1;
    if (obj1.job < obj2.job) return -1;
    return 0;
  });

  //console.log(solvedProcessesInfo);
};

function srtf(processId: number[], arrivalTime: number[], burstTime: number[]) {
  const processesInfo = processId
    .map((item, index) => {
      return {
        job: item,
        at: arrivalTime[index],
        bt: burstTime[index],
      };
    })
    .sort((obj1, obj2) => {
      if (obj1.at > obj2.at) return 1;
      if (obj1.at < obj2.at) return -1;
      if (obj1.bt > obj2.bt) return 1;
      if (obj1.bt < obj2.bt) return -1;
      return 0;
    });

  const solvedProcessesInfo = [];
  //const ganttChartInfo: ganttChartInfoType = [];

  const readyQueue = [];
  let currentTime = processesInfo[0].at;
  const unfinishedJobs = [...processesInfo];

  const remainingTime = processesInfo.reduce((acc, process) => {
    acc[process.job] = process.bt;
    return acc;
  }, {});

  readyQueue.push(unfinishedJobs[0]);
  while (
    Object.values(remainingTime).reduce((acc: number, cur: number) => {
      return acc + cur;
    }, 0) &&
    unfinishedJobs.length > 0
  ) {
    let prevIdle = false;
    if (
      readyQueue.length === 0 &&
      unfinishedJobs.length > 0
    ) {
      prevIdle = true;
      readyQueue.push(unfinishedJobs[0]);
      console.log(`At time ${unfinishedJobs[0].at} process ${unfinishedJobs[0].job} started arr ${unfinishedJobs[0].at} total ${unfinishedJobs[0].bt} remain ${unfinishedJobs[0].bt} wait 0`);
      
    }

    readyQueue.sort((a, b) => {
      // Equal-priority processes are scheduled in FCFS order.
      if (remainingTime[a.job] > remainingTime[b.job]) return 1;
      if (remainingTime[a.job] < remainingTime[b.job]) return -1;
      if (a.job < b.job)
        return -1;
      return 1;
    });

    const processToExecute = readyQueue[0];

    const processATLessThanBT = processesInfo.filter((p) => {
      let curr: number = currentTime;
      if (prevIdle) {
        curr = processToExecute.at;
      }

      return (
        p.at <= remainingTime[processToExecute.job] + curr &&
        p !== processToExecute &&
        !readyQueue.includes(p) &&
        unfinishedJobs.includes(p)
      );
    });

    let gotInterruption = false;
    processATLessThanBT.some((p) => {
      if (prevIdle) {
        currentTime = processToExecute.at;
      }

      const amount = p.at - currentTime;

      if (currentTime >= p.at) {
        readyQueue.push(p);
      }
      //? what is this   ans stopped
      if (p.bt < remainingTime[processToExecute.job] - amount) {
        remainingTime[processToExecute.job] -= amount;
        readyQueue.push(p);
        const prevCurrentTime = currentTime;
        currentTime += amount;

        //console.log(`At time ${currentTime} process ${processToExecute.job} stopped arr ${processToExecute.at} total ${processToExecute.bt} remain 0 wait ${currentTime - processToExecute.at - processToExecute.bt}`);
        // ganttChartInfo.push({
        //   job: processToExecute.job,
        //   start: prevCurrentTime,
        //   stop: currentTime
        // });
        
        gotInterruption = true;
        return true;
      }
    });
    const processToArrive= processesInfo.filter((p) => {
      return (
        p.at <= currentTime &&
        p !== processToExecute &&
        !readyQueue.includes(p) &&
        unfinishedJobs.includes(p)
      );
    });

    // Push new processes to readyQueue
    readyQueue.push(...processToArrive);

    if (!gotInterruption) {
      if (prevIdle) {
        const remainingT = remainingTime[processToExecute.job];
        remainingTime[processToExecute.job] -= remainingT;
        currentTime = processToExecute.at + remainingT;

        processATLessThanBT.forEach(p => {
          if (currentTime >= p.at) {
            readyQueue.push(p);
          }
        });

        //console.log(`At time ${processToExecute.at} process ${processToExecute.job} resumed arr ${processToExecute.at} total ${processToExecute.bt} remain ${processToExecute.bt} wait 0`);
        // ganttChartInfo.push({
        //   job: processToExecute.job,
        //   start: processToExecute.at,
        //   stop: currentTime
        // });
      } else {
        const remainingT = remainingTime[processToExecute.job];
        remainingTime[processToExecute.job] -= remainingT;
        const prevCurrentTime = currentTime;
        currentTime += remainingT;

        processATLessThanBT.forEach(p => {
          if (currentTime >= p.at && !readyQueue.includes(p)) {
            readyQueue.push(p);
          }
        });

        //console.log(`At time ${prevCurrentTime} process ${processToExecute.job} started arr ${processToExecute.at} total ${processToExecute.bt} remain ${processToExecute.bt} wait ${prevCurrentTime - processToExecute.at}`); 
        // ganttChartInfo.push({
        //   job: processToExecute.job,
        //   start: prevCurrentTime,
        //   stop: currentTime
        // });
      }      
    }

    // Requeueing (move head/first item to tail/last)
    readyQueue.push(readyQueue.shift());
    
    // When the process finished executing
    if (remainingTime[processToExecute.job] === 0) {
      const indexToRemoveUJ = unfinishedJobs.indexOf(processToExecute);
      if (indexToRemoveUJ > -1) {
        unfinishedJobs.splice(indexToRemoveUJ, 1);
      }
      const indexToRemoveRQ = readyQueue.indexOf(processToExecute);
      if (indexToRemoveRQ > -1) {
        readyQueue.splice(indexToRemoveRQ, 1);
      }

      solvedProcessesInfo.push({
        ...processToExecute,
        ft: currentTime,
        tat: currentTime - processToExecute.at,
        wat: currentTime - processToExecute.at - processToExecute.bt,
      });
      console.log(`At time ${currentTime} process ${processToExecute.job} finished arr ${processToExecute.at} total ${processToExecute.bt} remain 0 wait ${currentTime - processToExecute.at - processToExecute.bt} TA ${currentTime - processToExecute.at} WTA ${safeDiv((currentTime - processToExecute.at), (processToExecute.bt))}`);
    }
  }

  // Sort the processes by job name within arrival time
  solvedProcessesInfo.sort((obj1, obj2) => {
    if (obj1.at > obj2.at) return 1;
    if (obj1.at < obj2.at) return -1;
    if (obj1.job > obj2.job) return 1;
    if (obj1.job < obj2.job) return -1;
    return 0;
  });

  return solvedProcessesInfo
};
import { readFileSync } from 'fs';

function isBlank(str : string) {
	return (!str || /^\s*$/.test(str));
}
const file = readFileSync('./processes.txt', 'utf-8');
const lines = file.toString().replace(/\r\n/g, '\n').split('\n');
const processId: number[] = [];
const arrivalTime: number[] = [], burstTime: number[] = [], priority: number[] = [];
for(let line of lines) {
	if(line.startsWith('#') || isBlank(line))
		continue;
	const split = line.trim().split(/\s+/);
	processId.push(Number(split[0])); 
	arrivalTime.push(Number(split[1])); 
	burstTime.push(Number(split[2])); 
	priority.push(Number(split[3])); 
}
console.log("#At time x process y state arr w total z remain y wait k");
if(args[2] === "1")
  npp(processId, arrivalTime, burstTime, priority);
if(args[2] === "2")
  srtf(processId, arrivalTime, burstTime);
if(args[2] === "3")
  rr(processId, arrivalTime, burstTime, Number(args[3]));

	
