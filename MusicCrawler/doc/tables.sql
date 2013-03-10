-- 歌手ID可以当作专辑和歌曲的外键
-- 歌曲不一定属于某张专辑，所以专辑ID不能作为歌曲的外键

CREATE TABLE SINGERS (
	singer_id INT NOT NULL PRIMARY KEY,
	singer_name VARCHAR(255) NOT NULL,
	type VARCHAR(32) NOT NULL,
	listeners INT NOT NULL DEFAULT 0,
	region VARCHAR(32),
	birthday VARCHAR(32)
) CHARSET=UTF8;

CREATE TABLE ALBUMS (
	album_id INT NOT NULL PRIMARY KEY,
	album_name VARCHAR(255) NOT NULL,
	singer_id INT NOT NULL,
	singer_name VARCHAR(255) NOT NULL,
	release_time DATE,
	styles VARCHAR(255),
	company VARCHAR(32),
	CONSTRAINT albums_singers FOREIGN KEY (singer_id) REFERENCES SINGERS (singer_id) ON UPDATE CASCADE ON DELETE CASCADE
) CHARSET=UTF8;

CREATE TABLE SONGS (
	song_id INT NOT NULL PRIMARY KEY,
	song_name VARCHAR(512) NOT NULL,
	album_id INT,
	album_name VARCHAR(255),
	singer_id INT NOT NULL,
	singer_name VARCHAR(255) NOT NULL,
	hot_number INT NOT NULL DEFAULT 0,
	release_time DATE,
	company VARCHAR(32),
	tags VARCHAR(255),
	CONSTRAINT songs_singers FOREIGN KEY (singer_id) REFERENCES SINGERS(singer_id) ON UPDATE CASCADE ON DELETE CASCADE
) CHARSET=UTF8;