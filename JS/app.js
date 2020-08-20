window.onscroll = function () {
  scrollFunc();
};

var menu = document.getElementById("menu");
var sticky = menu ? menu.offsetTop : undefined;

function scrollFunc() {
  if (!sticky) return;

  if (window.pageYOffset >= sticky) menu.classList.add("sticky");
  else menu.classList.remove("sticky");
}
