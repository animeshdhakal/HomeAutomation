file = open("source.html", "r")

page = file.read()

print(page.find("<!--"))

print(page[page.find("<!--")])

for i in page.rfind("<!--"):
    print(i)