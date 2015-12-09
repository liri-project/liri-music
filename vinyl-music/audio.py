#coding=utf-8 
import sys, os
from mutagen.easyid3 import EasyID3
from mutagen.mp3 import MP3
import sqlite3 as lite

def getSongObject(path, name):
	audio = MP3(path, ID3=EasyID3)
	try:
		return (audio['title'][0], path, audio['album'][0], audio['artist'][0])
		#return {'title': audio['title'], 'path': path, 'album': audio['album'], 'artist': audio['artist']}
	except:
		return (name, path, 'Unknown Album', 'Unknown Artist')
		#return {'title': name, 'path': path, 'album': 'Unknown Album', 'artist': 'Unknown Artist'}

def getMusic():
	home = os.path.expanduser('~')
	songs = []
	for root, dirs, files in os.walk(home + '/Music'):
		rootpath = root
		for name in files:
			if name.endswith((".mp3")):
				filepath = rootpath + '/' + name
				songs.append(getSongObject(filepath, name))
	return songs
	
def initialScan():
	songs = getMusic()
	con = lite.connect('main.db')

	try:
		cur = con.cursor()
		cur.execute("CREATE TABLE IF NOT EXISTS SONGS(id INTEGER PRIMARY KEY AUTOINCREMENT, Title TEXT, Path TEXT, Album TEXT, Artist TEXT)")
		cur.executemany("INSERT INTO SONGS VALUES (NULL, ?, ?, ?, ?)", songs)
		con.commit()
	except:
		print("Error entering db data")
		
def getAllSongs():
	con = lite.connect('main.db')
	cur = con.cursor()
	cur.execute("SELECT * FROM SONGS")
	rows = cur.fetchall()
	s = []
	
	try:
		for data in rows:
			nd = {'id': data[0], 'title': data[1], 'path': data[2], 'album': data[3], 'artist': data[4]}
			s.append(nd)
		print(s)

	except Exception as e:
		print("Error %s:" % e.args[0])
		sys.exit(1)
    
	finally:
		if con:
			con.close()
			
getAllSongs()
