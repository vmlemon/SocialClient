<CENTER> 
<FORM NAME="marquee1"> 
<INPUT NAME="text" SIZE=25
VALUE=" Welcome to JK's ultimate JavaScript tutorial! Have fun!"
> 
</FORM> 
</CENTER>
<SCRIPT> 
<!-- 
/*Text box marquee by Dave Methvin,
Windows Magazine
May be used/modified if credit line is
retained*/
ScrollSpeed = 100
ScrollChars = 1
function ScrollMarquee() {
window.setTimeout('ScrollMarquee()',ScrollSpeed);

var msg = document.marquee1.text.value; 
document.marquee1.text.value =
msg.substring(ScrollChars) +
msg.substring(0,ScrollChars); 
} 
ScrollMarquee()
//-->
</SCRIPT>