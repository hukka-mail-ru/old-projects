mysqldump -u root -proot --database ClockShop > dump.sql

echo "dumping to dump.sql"
perl -i -p -e 's/CREATE DATABASE/\-\- CREATE DATABASE/g' dump.sql
perl -i -p -e 's/USE \`ClockShop\`;/USE \`db67829m\`;/g' dump.sql

cp dump.sql ~/temp
echo "copied to Shared"
