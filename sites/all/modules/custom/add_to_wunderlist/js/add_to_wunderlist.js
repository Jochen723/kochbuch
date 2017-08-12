function searchsomething(zahl) {

    var zahl = document.getElementsByName('quantity')[0].value;

    if(zahl <50) {
        var counter = 0;
        var test = 'edit-ios-'+counter;

        while (document.getElementById(test)) {
            //alert(counter);

            var label = document.querySelector("#"+test+"+ label").innerText;

            var space = label.indexOf(' ');

            if(space >0) {

                var rechnung = label.substr(0,space);
                //rechnung = rechnung / zahl * (zahl+1);


                rechnung = (rechnung/zahl*(parseInt(zahl)+1));


                document.querySelector("#"+test+"+ label").innerText = rechnung +" "+ label.substr(space+1);





            }

            counter++;
            test = 'edit-ios-'+counter;





        }

        document.getElementsByName('quantity')[0].value ++;
    }
    }


function searchsomething2(zahl) {

    var zahl = document.getElementsByName('quantity')[0].value;

    if(zahl >1) {
        var counter = 0;
        var test = 'edit-ios-'+counter;

        while (document.getElementById(test)) {

            var label = document.querySelector("#"+test+"+ label").innerText;

            var space = label.indexOf(' ');

            if(space >0) {

                var rechnung = label.substr(0,space);
                //rechnung = rechnung / zahl * (zahl+1);


                rechnung = (rechnung/zahl*(parseInt(zahl)-1));


                document.querySelector("#"+test+"+ label").innerText = rechnung +" "+ label.substr(space+1);





            }

            counter++;
            test = 'edit-ios-'+counter;





        }

        document.getElementsByName('quantity')[0].value --;
    }

}





