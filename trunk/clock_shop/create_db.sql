CREATE DATABASE IF NOT EXISTS ClockShop CHARACTER SET cp1251;
GRANT ALL PRIVILEGES ON ClockShop.* TO 'hukka'@'localhost' IDENTIFIED BY '777' WITH GRANT OPTION;
GRANT ALL ON ClockShop.* TO hukka@localhost;
FLUSH PRIVILEGES;