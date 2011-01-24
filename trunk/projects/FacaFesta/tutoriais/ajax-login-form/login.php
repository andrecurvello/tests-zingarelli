<?php
require_once 'config.php';

// Is the user already logged in? Redirect him/her to the private page

if(isSet($_SESSION['username']))
{
header("Location: private.php");
exit;
}
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
 <HEAD>
  <TITLE>AJAX Login Form</TITLE>

  <META name="Author" Content="Bit Repository">
  <META name="Keywords" Content="ajax, login, form, mootools">
  <META name="Description" Content="A nice & simple AJAX Login Form">

  <script type="text/javascript" src="js/mootools-1.2.1-core-yc.js"></script>
  <script type="text/javascript" src="js/process.js"></script>

  <link rel="stylesheet" type="text/css" href="style.css" />
</HEAD>

 <BODY>
 
<center>

<div id="status">

<fieldset><legend align="center">Authentication</legend>

<div id="login_response"><!-- spanner --></div>

<form id="login" name="login" method="post" action="do_login.php">
<table align="center" width="300" border="0">
<tr>
<td width="80">Username</td><td><input id="name" type="text" name="username"></td>
</tr>
<tr>
<td>Password</td>
<td><input type="password" name="password"></td>
</tr>
<tr>
<td>&nbsp;</td>
<td><input type="checkbox" name="autologin" value="1">Remember Me</td>
</tr>
<tr>
<td>&nbsp;</td>
<td><input id="submit" type="submit" name="submit" value="Login">
<div id="ajax_loading"><img align="absmiddle" src="images/spinner.gif">&nbsp;Processing...</div></td>
</tr>
</table>
</form>
</fieldset>

</div>
</center>
 </BODY>
</HTML>