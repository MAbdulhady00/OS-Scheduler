import sys
out = open('out.log', 'r')
compare1 = list()
for line in out:
    if(line.find('finished') == -1):
        continue
    compare1.append(line)

scheduler = open('scheduler.log', 'r')
compare2 = list()
for line in scheduler:
    if(line.find('finished') == -1):
        continue
    compare2.append(line)

error = False 
for (i, string) in enumerate(compare1):
    if(string != compare2[i]):
        print(f'< {compare2[i]}> {string}')
        error = True

if(not error):
    print(sys.argv[1])