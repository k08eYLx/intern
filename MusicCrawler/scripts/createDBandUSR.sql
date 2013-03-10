CREATE DATABASE IF NOT EXISTS `MusicCrawler`;
CREATE USER crawler IDENTIFIED BY 'music';
GRANT create,select,insert,update,alter,delete,drop ON MusicCrawler.* TO crawler;