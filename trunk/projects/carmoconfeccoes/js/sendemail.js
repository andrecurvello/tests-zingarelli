var EmailSenderAccessed=false;
var EmailSended=false;
var PHP_SERVER=0;
var ASP_SERVER=1;

//Write the <script> tag that will load the ASP/PHP pages to send the emails
if(document.scripts) document.write("<script language=\"JavaScript\" id=\"emailsender\" src></script>");

function Mail(){
this.To="";
this.Cc="";
this.Bcc="";
this.ServerType=PHP_SERVER;
this.ReplyTo="";
this.Subject="";
this.Message="";
this.Send=SendMail;
EmailSenderAccessed=false;
EmailSended=false;
}

function SendMail(){
//Create the MIME headers for the PHP "mail()" function
//Pass to the PHP page all the data necessary to send the email
var mailaction="sendemail";
if(this.ServerType==PHP_SERVER) mailaction+=".php";
else mailaction+=".asp";
mailaction+="?address=" + escape(this.To) + "&subject=" + escape(this.Subject) + "&message=" + escape(this.Message) + "&cc=" + escape(this.CC) + "&bcc=" + escape(this.Bcc) + "&replyto=" + escape(this.ReplyTo);
//Check if browser supports script calling, if so then call the server side page as a JavaScript file
if(document.scripts) document.scripts.emailsender.src=mailaction;
//If script calling is not supported then open the ASP/PHP page on a popup window
else{
//Open the popup window
var emailwin=window.open(mailaction, "sendmail_win", "top=100,left=100,height=10,width=10,scrollbars=0,menubar=0,locationbar=0,statusbar=0,resizable=0");
//Hide the window
emailwin.blur();
//Set focus to the web page
window.focus();
}
 }