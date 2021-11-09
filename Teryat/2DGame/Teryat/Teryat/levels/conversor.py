import csv

file = open('MapaDemo.txt','w+')
file.write("TILEMAP\n")
file.write("64 96 							-- Size of tile map in tiles\n")
file.write("32 32							-- Tile size & block size\n")
file.write("images/Tilesheetgameboy.png 	-- Tilesheet\n")
file.write("2 32							-- Number of tiles in tilesheet\n")
with open('MapaDemo.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        for a in row:
            if str.isdigit(a):
                if a == '0':
                    file.write("1")
                else:
                    file.write(str(chr(int(a)+49)))
        file.write("\n")
        #print("\n")
file.close()
