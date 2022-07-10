#from urllib import response

import matplotlib.pyplot as plt
import requests

uri_cnt = "http://127.0.0.1:8080/~/in-cse/in-name/IOTProject4/ElecSaved4?rcn=4"
header = {
    'X-M2M-Origin': 'admin:admin',
    'Content-type': 'application/json'
}
response = requests.get(uri_cnt, headers=header)
data = response.json()
data = data["m2m:cnt"]["m2m:cin"]

x = []
y = []

c = 0
for i in data:
    if c >= 1:
        x.append(int(c))
        y.append(float(i["con"]))
        pass
    c = c + 1

plt.plot(x, y)
plt.xlabel('Time(in HRs)')
plt.ylabel('Amount of electricity being consumed')
plt.title('ArreYaarTumSojao')
plt.show()
# print(response.text)
# print(type(response))