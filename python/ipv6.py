import re

def get_ipv6_network(ip_address_with_mask):
    m = re.search('([^/]+)/(\d+)',ip_address_with_mask)
    if m:
        ip_address = m.group(1)
        mask = int(m.group(2))
    ip_address = expand_ipv6_address(ip_address)
    ip_address_list = ['{:04X}'.format(int(element,16)) for element in ip_address.split(':')]
    mask_list = []
    halfword = mask
    while (halfword >= 16):
        mask_list.append(hex(int('FFFF',16)))
        halfword = halfword - 16
    if halfword != 0:
        print int(65535 - (2 ** (16-halfword) - 1))
        print hex(int(65535 - (2 ** (16-halfword) - 1)))
        mask_list.append(hex(int(65535 - (2 ** (16-halfword) - 1))))
    while (len(mask_list) != 8):
        mask_list.append(hex(int('0000',16)))
    network_address_list = []
    for count in range(0,8):
        network_address_list.append('{:04X}'.format((int(ip_address_list[count],16)) & (int(mask_list[count],16))))
    network_address = ':'.join(network_address_list)
    return network_address

def get_ipv6_network1(ip_address_with_mask):
    m = re.search('([^/]+)/(\d+)',ip_address_with_mask)
    if m:
        ip_address = m.group(1)
        mask = int(m.group(2))
    ip_address = expand_ipv6_address(ip_address)


def expand_ipv6_address(ip_address):
    m = re.search('::',ip_address)
    if not m:
        print "expand_ipv6_address already expanded address"
        return ip_address
    else:
        input_ip_address = re.sub('::',':',ip_address)
        input_address_list = filter(None,input_ip_address.split(':'))
        input_hexgroup = len(input_address_list)
        pending_hexgroup = 8 - input_hexgroup
        new_group = []
        for count in range(0,pending_hexgroup):
            new_group.append('0000')
        new_group = ':'.join(new_group)
        new_group = ':' + new_group + ':'
        ip_address = ip_address.replace('::',new_group).strip(':')
        ip_address = ':'.join(['{:04X}'.format(int(element,16)) for element in ip_address.split(':')])
        #ip_address = re.sub('::',new_group,ip_address).strip(':')
        return ip_address

def linklocalfrommac(mac):
    new_list = mac.split(':')
    new_list.insert(3,'ff')
    new_list.insert(4,'fe')
    new_list[0] = '{:0x}'.format((int(new_list[0],16) ^ 2))
    result = ""
    for count in [0,2,4,6]:
        result = result + ':' + new_list[count] + new_list[count+1]
    result = result.strip(':')
    result = 'FE80::' + result
    return result

def linklocaltomac(ipv6_address):
    ipv6_mac = ipv6_address.split('::')[1]
    ipv6_mac = re.sub(':','',ipv6_mac)
    mac_list = []
    for count in range(0,len(ipv6_mac),2):
        mac_list.append(ipv6_mac[count]+ipv6_mac[count+1])
    mac_list[0] = '{:0x}'.format((int(mac_list[0],16) ^ 2))
    mac_list.pop(3)
    mac_list.pop(3)
    result = ':'.join(mac_list)
    return result

def increment_ipv6(ipv6_address,step):
    ipv6 = expand_ipv6_address(ipv6_address)
    step = expand_ipv6_address(step)
    ipv6_list = ipv6.split(':')
    step_list = step.split(':')
    carry = 0
    result_list = []
    for index in range(7,-1,-1):
        value = int(ipv6_list[index],16) + int(step_list[index],16) + carry
        carry = value / 65535
        value = value % 65535
        result_list.append('{:0x}'.format(value))
    result_list.reverse()
    result = ':'.join(result_list)
    return result

def contract(ipv6):
    #return re.sub(r'(?:^|:)0(?::0)+(?::|$)','::',ipv6,1)
    ipv6 = ':'.join(['{:x}'.format((int(element,16))) for element in ipv6.split(':')])
    return re.sub(r'(^|:)0(:0)+(:|$)','::',ipv6,1)

#print get_ipv6_network("11:11:11:abcd::1/62")
#print expand_ipv6_address("11::1")
#print expand_ipv6_address("11:1::1:1")
#print expand_ipv6_address("11::1:1")
#print expand_ipv6_address("::1")
#print expand_ipv6_address("1::")
#print expand_ipv6_address("::")
#print linklocaltomac(linklocalfrommac('52:74:f2:b1:a8:7f'))
#print increment_ipv6('11::1','0:1::')
#print contract(expand_ipv6_address("11::1"))
print contract("11:0:0000:0:0:0:0:1")
print contract("11:0:0:0:1:0:0:1")
