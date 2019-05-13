<?php
require "../usertools.php";
if (checkLogin() == null) {
    header("Location: index.php");
    exit();
}
?>
<!DOCTYPE html>
<html>
  <head>
  <title>kidO dashboard</title>
  
  <meta http-equiv="refresh" content="10" > 
  
  <style>
  
  .container-table100 {
    width: 100%;
    min-height: 100vh;
    background: #c850c0;
    background: -webkit-linear-gradient(45deg, #4158d0, #c850c0);
    background: -o-linear-gradient(45deg, #4158d0, #c850c0);
    background: -moz-linear-gradient(45deg, #4158d0, #c850c0);
    background: linear-gradient(45deg, #4158d0, #c850c0);

    display: -webkit-box;
    display: -webkit-flex;
    display: -moz-box;
    display: -ms-flexbox;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-wrap: wrap;
    padding: 33px 30px;
  }
  .wrap-table100 {
    width: 90%;
    margin-top: 50px;
    margin-left: 5%;
  }
  @media only screen and (min-width: 800px) {
    .wrap-table100 {
      width: 40%;
      margin-top: 50px;
      margin-left: 33%;
    }
  }
  table {
    border-spacing: 1;
    border-collapse: collapse;
    background: white;
    opacity:0.9;
    border-radius: 10px;
    overflow: hidden;
    width: 100%;
    position: relative;
    align: center;
  }
  table * {
    position: relative;
  }
  table td, table th {
    padding-left: 8px;
  /*  align-content: 1px;*/
  }
  table thead tr {
    height: 60px;
    background: #36304a;
  }
  table tbody tr {
    height: 50px;
  }
  table tbody tr:last-child {
    border: 0;
  }
  table td, table th {
    text-align: left;
  }
  table td.l, table th.l {
    text-align: right;
  }
  table td.c, table th.c {
    text-align: center;
  }
  table td.r, table th.r {
    text-align: center;
  }

  h1 {
    font-family: OpenSans-Regular;
    font-size: 40px;
    color: #fff;
    line-height: 1.2;
    font-weight: unset;
    margin-left: 43%;
  }
   h3 {
      font-family: OpenSans-Regular;
      font-size: 20px;
      color: #fff;
      line-height: 1.2;
      font-weight: unset;
    }
  button {
    font-family: OpenSans-Regular;
    font-size: 18px;
    line-height: 1.2;
    font-weight: unset;
    margin-left: 2%;
    border-radius: 5px;
    box-shadow: 0 10px 30px 0px rgba(0, 0, 0, 0.1);
    -moz-box-shadow: 0 10px 30px 0px rgba(0, 0, 0, 0.1);
    -webkit-box-shadow: 0 10px 30px 0px rgba(0, 0, 0, 0.1);
    -o-box-shadow: 0 10px 30px 0px rgba(0, 0, 0, 0.1);
    -ms-box-shadow: 0 10px 30px 0px rgba(0, 0, 0, 0.1);
  }
  
  .table100-head th{
    font-family: OpenSans-Regular;
    font-size: 18px;
    color: #fff;
    line-height: 1.2;
    font-weight: unset;
  }

  @font-face {
    font-family: OpenSans-Regular;
    src: url('fonts/OpenSans/OpenSans-Regular.ttf'); 
  }

  tbody tr {
    font-family: OpenSans-Regular;
    font-size: 15px;
    color: #808080;
    line-height: 1.2;
    font-weight: unset;
  }

  tbody tr:hover {
    color: #555555;
    font-size: 17px;
    background-color: #f5f5f5;
    cursor: pointer;
  }  
  
  </style>
  </head>
  
  <body style = " height:100% background: #c850c0; background: -webkit-linear-gradient(45deg, #4158d0, #c850c0); background: -o-linear-gradient(45deg, #4158d0, #c850c0); background: -moz-linear-gradient(45deg, #4158d0, #c850c0); background: linear-gradient(45deg, #4158d0, #c850c0);">
    <script>
      function logout() {
        document.cookie = "username= "
        document.cookie = "password= " 
        open("login.php","_self");
      }
    </script>
    <button onclick = "logout()">log out</button>
    
    <h1>kidO - dashboard</h1>
      <div class="wrap-table100">
        <table>
          <?php
            $username = checkLogin();
            $content = content("../files/".$username.".csv");
            if ($content == "none"){
              return;
            }
            $max = 0;
            foreach ($content as $line) {
                $commas = count($line);
                if ($max < $commas + 1) {
                    $max = $commas + 1;
                }
            }
            foreach ($content as $line) {
                echo "<tr>";
                for ($cell = 0; $cell < $max; $cell++) {
                    if ($cell < count($line)) {
                        echo "<td>".$line[$cell]."</td>";
                    } else {
                        echo "<td></td>";
                    }
                }
                echo "</tr>";
            }
          ?>
        </table>
      </div>
  </body>
</html>