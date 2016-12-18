n=5
for i in range(n,0,-1):
    for j in range(1,i):
        print '.',
    print n-i+1,
    for j in range(n,i,-1):
        print '.',
    print ''
