CREATE TABLE IF NOT EXISTS SINGERS (
	singer_id INT NOT NULL PRIMARY KEY,
	singer_name VARCHAR(255) NOT NULL,
	type VARCHAR(32) NOT NULL,
	listeners INT NOT NULL DEFAULT 0,
	region VARCHAR(32),
	birthday VARCHAR(32)
) CHARSET=UTF8;

CREATE TABLE IF NOT EXISTS SINGERS_EXT (
	singer_id INT NOT NULL PRIMARY KEY,
	singer_name VARCHAR(255) NOT NULL,
	f_cn_char_id BIGINT,
	s_cn_char_id BIGINT,
	t_cn_char_id BIGINT,
	type VARCHAR(32) NOT NULL,
	listeners INT NOT NULL DEFAULT 0,
	region VARCHAR(32),
	birthday VARCHAR(32)
) CHARSET=UTF8;

CREATE TABLE IF NOT EXISTS ALBUMS (
	album_id INT NOT NULL PRIMARY KEY,
	album_name VARCHAR(255) NOT NULL,
	singer_id INT NOT NULL,
	singer_name VARCHAR(255) NOT NULL,
	release_time DATE,
	styles VARCHAR(255),
	company VARCHAR(32),
	FOREIGN KEY (singer_id) REFERENCES SINGERS(singer_id) ON DELETE CASCADE
) CHARSET=UTF8;

CREATE TABLE IF NOT EXISTS ALBUMS_EXT (
	album_id INT NOT NULL PRIMARY KEY,
	album_name VARCHAR(255) NOT NULL,
	f_cn_char_id BIGINT,
	s_cn_char_id BIGINT,
	t_cn_char_id BIGINT,
	singer_id INT NOT NULL,
	singer_name VARCHAR(255) NOT NULL,
	release_time DATE,
	styles VARCHAR(255),
	company VARCHAR(32),
	FOREIGN KEY (singer_id) REFERENCES SINGERS_EXT(singer_id) ON DELETE CASCADE
) CHARSET=UTF8;

CREATE TABLE IF NOT EXISTS SONGS (
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
	FOREIGN KEY (singer_id) REFERENCES SINGERS(singer_id) ON DELETE CASCADE
) CHARSET=UTF8;

CREATE TABLE IF NOT EXISTS SONGS_EXT (
	song_id INT NOT NULL PRIMARY KEY,
	song_name VARCHAR(512) NOT NULL,
	f_cn_char_id BIGINT,
	s_cn_char_id BIGINT,
	t_cn_char_id BIGINT,
	album_id INT,
	album_name VARCHAR(255),
	singer_id INT NOT NULL,
	singer_name VARCHAR(255) NOT NULL,
	hot_number INT NOT NULL DEFAULT 0,
	release_time DATE,
	company VARCHAR(32),
	tags VARCHAR(255),
	FOREIGN KEY (singer_id) REFERENCES SINGERS_EXT(singer_id) ON DELETE CASCADE
) CHARSET=UTF8;
