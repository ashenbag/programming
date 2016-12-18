
def simple_solution():
    multiple_list = []
    for num in range(0,1000):
        if num % 3 == 0 or num % 5 == 0:
            multiple_list.append(num)

    sum_value = 0
    for item in multiple_list:
        sum_value += item

    print sum_value

simple_solution()

def sum_divisible_by_n(num):
    temp = int(1000/num)
    return (num * ((temp * (temp + 1))/2))

def good_solution():
    print sum_of_n(3) + sum_of_n(5) - sum_of_n(15)
