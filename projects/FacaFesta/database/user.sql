CREATE TABLE ff_usr(
	usr_id integer NOT NULL AUTO_INCREMENT,
	usr_login varchar(15) NOT NULL,
	usr_pwd varchar(50) NOT NULL,
	usr_name varchar(80) NOT NULL,
	usr_cpf integer NOT NULL,
	usr_city varchar(80) NOT NULL,
	usr_state char(2) NOT NULL,
	usr_status integer NOT NULL,
	usr_created datetime NOT NULL,
	CONSTRAINT uc_cpf UNIQUE (usr_cpf),
	CONSTRAINT pk_id PRIMARY KEY (usr_id)
);