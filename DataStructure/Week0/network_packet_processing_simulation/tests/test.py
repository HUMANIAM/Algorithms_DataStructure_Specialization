def shedule_packets(st_pt):
	finished_time = list()
	respones = list()
	finishedtime = 0
	buffsize = int(st_pt[0][0])
	count =  int(st_pt[0][1])
	i = 1
	if(count == 0):
		return;
	else:
		while(i <= count):

			#if the buffer has empty location
			if buffsize > 0:
				if(finishedtime < int(st_pt[i][0])):
					respones.append((1,st_pt[i][0]))
					finishedtime = int(st_pt[i][0]) + int(st_pt[i][1])
				else:
					respones.append((1,finishedtime))
					finishedtime = finishedtime + int(st_pt[i][1])
				finished_time.append(finishedtime)
				buffsize = buffsize - 1
				pass

			#if the buffer has no empty location but there are some packets has been processed
			elif finished_time[0] <= int(st_pt[i][0]):
				j = 0
				FTList = len(finished_time)
				#finishedtime = finishedtime + int(st_pt[i][1])
				while j<FTList :
					if finished_time[j] < int(st_pt[i][0]):
						j = j + 1
						buffsize = buffsize + 1
					elif finished_time[j] == int(st_pt[i][0]):
						respones.append((1 , finishedtime))
						finishedtime = finishedtime + int(st_pt[i][1])
						finished_time.append(finishedtime)
						finished_time = finished_time[j+1:]
						break
					else:
						respones.append((1 , finishedtime))
						finishedtime = finishedtime + int(st_pt[i][1])
						finished_time.append(finishedtime)
						finished_time = finished_time[j:]
						buffsize = buffsize - 1
						break

				if j == FTList:
					respones.append((1 , int(st_pt[i][0])))
					finishedtime = int(st_pt[i][0]) + int(st_pt[i][1])
					finished_time = [finishedtime]

			else:
				respones.append((0,-1))
				pass
			i = i + 1

	return respones
	pass

def printResponses(respones, fname):
	state = "false"
	i = 0
	fhandle = open(fname)
	if respones != None:
		for line in fhandle:
			if respones[i][0] == 1:
				if int(respones[i][1]) == int(line):
					print("true")
				else:
					print("false ",fname ,respones[i][1], line)
					exit()
			else:
				if int(respones[i][1]) == int(line):
					print("true")
				else:
					print("false", fname,respones[i][1], line)
					exit()
				pass
			i = i + 1

i = 20
st_pt = list()
fname = "0"
while i<23:
	try:
		fhandler = open(str(i))
		pass
	except Exception as e:
		print("can't open this file")
		raise e
	
	for line in fhandler:
		st_pt.append(line.split())
		pass
	respones = shedule_packets(st_pt)
	#print(respones)fname +
	printResponses( respones, str(i) + ".a" )
	st_pt = []
	i = i + 1;
	pass



