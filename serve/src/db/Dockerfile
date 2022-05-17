FROM postgres:12
#ADD pg_hba.conf /var/lib/postgresql/data/
#ADD postgresql.conf /var/lib/postgresql/data/
COPY init.sql /docker-entrypoint-initdb.d/
EXPOSE 5432