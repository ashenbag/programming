import time
import random
def b_search(input_list,num):
    low=0
    high=len(input_list)-1
    while(low<=high):
        mid=(low+high)/2
        if num == input_list[mid]:
            return mid
        elif num < input_list[mid]:
            high=mid-1
        elif num > input_list[mid]:
            low=mid+1
    return 0

def sum_check1(num,e_sum):
    input_list = range(1,num)
    for i in range(len(input_list)):
        for j in range(i,len(input_list)):
            if input_list[i] + input_list[j] == e_sum:
                print "i: %d, j: %d" % (i,j)
                return 1
    return 0

def sum_check2(num,e_sum):
    input_list = range(1,num)
    for i in range(len(input_list)):
        other = e_sum - i
        j = b_search(input_list[i+1:],other)
        if j:
            print "i: %d, j: %d" % (i,j)
            return 1
    return 0

def sum_check3(num,e_sum):
    input_list = range(1,num)
    i=0
    j=len(input_list)-1
    while(i<=j):
        if input_list[i] + input_list[j] == e_sum:
            print "i: %d, j: %d" % (i,j)
            print "v1: %d, v2: %d" % (input_list[i],input_list[j])
            return 1
        elif input_list[i] + input_list[j] < e_sum:
            i+=1
        elif input_list[i] + input_list[j] > e_sum:
            j-=1
    return 0

def sum_check4(num,e_sum):
    input_list = [int(random.random() * 1000) for n in range(0,num)]
    e_sum = input_list[num-2] + input_list[num-1]
    comp_dict = {}
    for num in input_list:
        if num in comp_dict:
            return 1
        else:
            comp_dict[e_sum-num] = num


start_time = time.time()
if sum_check3(10000,9000):
    print "match"
else:
    print "no match"
print("--- %s seconds ---" % (time.time() - start_time))
