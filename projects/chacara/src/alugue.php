<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Chácara Zingarelli | Contato</title>
<link rel="stylesheet" type="text/css" href="css/principal.css"/>
<meta name="description" content="Informações de contato para alugar a Chácara Zingarelli. Envie um email ou ligue." />
<meta name="keywords" content="aluguel de chácara araraquara, telefone chácara Araraquara, telefone chácara zingarelli" />
<script type="text/javascript">	
	var _gaq = _gaq || [];
	_gaq.push(['_setAccount', 'UA-10206494-2']);
	_gaq.push(['_trackPageview']);
	
	(function() {
	var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
	ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
	var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
	})();	
</script>
</head>
<body>
	<?php include 'menu.php'; ?>
    <div id="container">
    	<div id="contato-content">
            <div id="main-title">
            <h1>Entre em contato conosco!</h1>
            </div>
            <p>Para saber mais sobre a chácara, condições de pagamento e promoções, ou nos fazer uma visita, entre em contato conosco através do formulário ou telefones abaixo.</p>
            <br />
            <p>Telefone: (16) 3336-9590 ou (16) 3335-8433</p>
            <p>Celular: (16) 9992-6852</p>
            <p>Email: alugue@chacarazingarelli.com.br</p>
            <br />
			<?php 
            //security check to send email (based on article Preventing spam when using PHP's mail function by Ian Gilfillan
            //http://www.phpbuilder.com/columns/ian_gilfillan20060412.php3?page=1
            if(isset($_POST["enviar"])){
                //send email
                $to = "alugue@chacarazingarelli.com.br";
                $subject = "Mensagem postada pelo site";
                $telephone = $_POST["telefone"];
                $message = $_POST["mensagem"] . " Telefone: $telephone";
                $email = $_POST["email"]; //testar se for em branco
                
				echo $message;
				
                function is_valid_email($email) {		
                    return preg_match('#^[a-z0-9.!\#$%&\'*+-/=?^_`{|}~]+@([0-9.]+|([^\s]+\.+[a-z]{2,6}))$#si', $email);
                }
        
                function contains_bad_str($str_to_test) {
                    $bad_strings = array(
                    "content-type:"
                    ,"mime-version:"
                    ,"multipart/mixed"
                    ,"Content-Transfer-Encoding:"
                    ,"bcc:"
                    ,"cc:"
                    ,"to:"
                    );
        
                    foreach($bad_strings as $bad_string) {
                        if(eregi($bad_string, strtolower($str_to_test))) {
                        //possible attack caught                        
						mail($to, "Possível ataque recebido no site", "A seguinte mensagem foi marcada como alvo de possível ataque $str_to_test");
						echo '<p class=\'red\'>Desculpe, mas sua mensagem foi filtrada como tentativa de ataque ou spam. Por favor, entre em contato conosco pelo telefone ou <a href=\'alugue.php\' title=\'Vá para a página de contato\'>tente novamente</a>.</p></div>';
						include 'footer.php';
						echo'</div>';
						include 'webdeveloper.php';
                        exit;
                        }
                    }
                }
        
                function contains_newlines($str_to_test) {
                    if(preg_match("/(%0A|%0D|\\n+|\\r+)/i", $str_to_test) != 0) {
                        mail($to, "Possível ataque recebido no site", "A seguinte mensagem foi marcada como alvo de possível ataque $str_to_test");
						echo '<p class=\'red\'>Desculpe, mas sua mensagem foi filtrada como tentativa de ataque ou spam. Por favor, entre em contato conosco pelo telefone ou <a href=\'alugue.php\' title=\'Vá para a página de contato\'>tente novamente</a>.</p></div>';
						include 'footer.php';
						echo'</div>';
						include 'webdeveloper.php';
                        exit;
                    }
                } 
        
                if($_SERVER['REQUEST_METHOD'] != "POST"){
                    mail($to, "Possível ataque recebido no site", "Houve uma tentativa de enviar request sem post");
                    exit;
                }
        
                if(!empty($email)){
                    if (!is_valid_email($email)) {
						echo '<p class=\'red\'>Desculpe, mas o email digitado é inválido. Por favor, entre em contato conosco pelo telefone ou <a href=\'alugue.php\' title=\'Vá para a página de contato\'>tente novamente</a>.</p></div>';
						include 'footer.php';
						echo'</div>';
						include 'webdeveloper.php';
                        exit;
                    }
                }
                
                //verify input
                contains_bad_str($email);
                contains_bad_str($message);
                contains_bad_str($telephone);		
                contains_newlines($email);
                contains_newlines($message);
                contains_newlines($telephone);
                
                //send email
                $headers = "From: $email";
                if (mail($to, $subject, $message, $headers)){
					echo '<p>Sua mensagem foi enviada, obrigado!</p>';
				}
				else{
					echo "<p class='red'>Desculpe, mas não foi possível enviar sua mensagem devido a uma falha no servidor. Por favor, tente entrar em contato conosco pelos telefones e email acima. Obrigado!</p></div>";
					include 'footer.php';
					echo'</div>';
					include 'webdeveloper.php';
					exit;
				}
            }	
            else{ //show form
            ?>			
            <form id="contato" action="alugue.php" method="post">
            	<p><label for="email">Email:</label>
                <input type="text" id="email" name="email"/></p>
            	<p><label for="telefone">Telefone:</label>
                <input type="text" id="telefone" name="telefone"/></p>
            	<p><label for="mensagem">Mensagem:</label>
                <textarea id="mensagem" rows="5" cols="40" name="mensagem"></textarea></p>
                <p><input type="submit" value="Enviar" name="enviar"/></p>
            </form>
            <?php }?>
            <br />
            <h2>Aguardamos seu contato!!</h2>
	    </div>
		<?php include 'footer.php'; ?>
    </div>
    <?php include 'webdeveloper.php'?>	
</body>
</html>
