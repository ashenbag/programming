num = 10
print ""
for i in range(0,num):
    for j in range(0,num-i-1):
        print "",
    for k in range(0,i+1):
        print "*",
    print ""
for i in range(num,0,-1):
    for j in range(0,num-i+1):
        print "",
    for k in range(0,i-1):
        print "*",
    print ""
