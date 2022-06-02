
CREATE DATABASE aegis;


CREATE USER aegisadmin;
ALTER USER aegisadmin with PASSWORD 'aegisAdmin';

CREATE USER grafana;
ALTER USER grafana with PASSWORD 'grafanaUser';

CREATE USER nodered;
ALTER USER nodered with PASSWORD 'noderedUser';

\connect aegis

CREATE TABLE IF NOT EXISTS sensores(
	nome VARCHAR(20) PRIMARY KEY,
	endereco VARCHAR(50) NOT NULL,
	cidade VARCHAR(50) NOT NULL,
	bairro VARCHAR(50) NOT NULL,
	latitude numeric,
	longitude numeric	
);

CREATE TABLE IF NOT EXISTS dados_sensores (
	data_e_hora timestamp with time zone PRIMARY KEY,
	nome VARCHAR(20), 
    timetx numeric NOT NULL,
    timerx numeric NOT NULL,
    temperature numeric,
    humidity numeric,
    pressure numeric,
    pluviometric numeric,
	FOREIGN KEY(nome) REFERENCES sensores (nome)
);

INSERT INTO sensores(nome, endereco, cidade, bairro)
VALUES ('AEGIS' ,'não especificado','não especificado','não especificado');


GRANT ALL PRIVILEGES ON DATABASE AEGIS TO aegisadmin;
GRANT SELECT ON ALL TABLES IN SCHEMA public TO aegisadmin;
GRANT INSERT, UPDATE, DELETE ON  sensores to aegisadmin;

GRANT SELECT ON ALL TABLES IN SCHEMA public TO grafana;
GRANT SELECT, INSERT, UPDATE  ON ALL TABLES IN SCHEMA public TO nodered;