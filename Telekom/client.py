import sys
import json

path = sys.argv[1]
with open(path, 'r') as file:
    data = json.load(file)

counter = 0
usedPaths = list()
for i in range(data['simulation']['duration']):
    for alloc in data['simulation']['demands']:
        if alloc['start-time'] == i:
            found = False
            for path in data['possible-circuits']:
                if path[0] == alloc['end-points'][0] and path[len(path) - 1] == alloc['end-points'][1]:
                    wrong = False
                    for j in range(len(path) - 2):
                        for link in data['links']:
                            if path[j] == link['points'][0] and path[j+1] ==link['points'][1] and link['capacity'] < 10.0:
                                wrong = True
                                break
                        if wrong:
                            break
                    if not wrong:
                        for j in range(len(path) - 2):
                            for link in data['links']:
                                if path[j] == link['points'][0] and path[j+1] ==link['points'][1]:
                                    link['capacity'] -= 10.0
                        counter += 1
                        print(counter , '.igény foglalás: ' , alloc['end-points'][0] , '<->' , alloc['end-points'][1] , ' st:' , i , ' – sikeres', sep='')
                        usedPaths.append((alloc['end-time'], path, alloc['end-points']))
                        found = True
                        break
            if not found:
                counter += 1
                print(counter , '.igény foglalás: ' , alloc['end-points'][0] , '<->' , alloc['end-points'][1] , ' st:' , i , ' – sikertelen', sep='')
        
    for path in usedPaths:
        if path[0] == i:
            counter += 1
            print(counter , '.igény felszabadítás: ' , path[2][0] , '<->' , path[2][1] , ' st:' , i, sep='')
            for j in range(len(path[1]) - 2):
                    for link in data['links']:
                        if path[1][j:j+1] == link['points']:
                            link['capacity'] += 10