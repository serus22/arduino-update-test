'use strict'

const express = require('express');
const path = require('path');
const fs = require('fs');

const app = express();
const port = 5432;

let filename = '01.ino.nodemcu.bin';

let firmware = path.resolve(__dirname, './01.ino.nodemcu.bin');

app.get('/*', (req, res) => {
  
  var file = fs.createReadStream(firmware);
  var stat = fs.statSync(firmware);

  res.setHeader('Content-type', 'application/octet-stream');
  res.setHeader('Content-disposition', `attachment; filename=${filename}`);
  res.setHeader('Content-Transfer-Encoding', 'binary');
  res.setHeader('Content-Length', stat.size);

  file.pipe(res);

  console.log('GET::' + req.url); 
  console.log(req.headers);
  
});

app.listen(port, () => console.log(`Example app listening on port ${port}!`))