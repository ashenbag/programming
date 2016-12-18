
def fibonnaci():
    p1 = 1
    p2 = 2
    #print p1
    #print p2
    sum_value = p2
    while(sum_value <= 4000000):
        new_value = p1 + p2
        if new_value % 2 == 0:
            sum_value += new_value
        p1=p2
        p2=new_value

    print sum_value

fibonnaci()
