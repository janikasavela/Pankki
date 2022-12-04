const express = require('express');
const router = express.Router();
const tili = require('../models/tili_model');

router.get('/:id', function(request,response){
    const id=request.params.id;
    tili.getSaldo(id,function(err,dbResult){
        if(err){
            response.json(err);
        }
        else{
            response.json(dbResult);
        }
    })
});

router.get('/checkOmistaja/:id', function(request,response){
    const id=request.params.id;
    tili.checkOmistaja(id,function(err,dbResult){
        if(err){
            response.json(err);
        }
        else{
            response.json(dbResult);
        }
    })
});


module.exports=router;