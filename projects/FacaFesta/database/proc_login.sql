DELIMITER //
CREATE PROCEDURE verifyLogin(
    in_login varchar(15),
    in_pwd varchar(50),
    OUT out_name varchar(80)
)
	BEGIN
		DECLARE l_login varchar(15);
		DECLARE l_pwd varchar(50);
		
		SET l_login = in_login;
		SET l_pwd = in_pwd;
		
		SELECT usr_name INTO out_name 
		FROM ff_usr AS usuario
		WHERE usuario.usr_login = l_login AND usuario.usr_pwd = l_pwd;
	END //
DELIMITER ;