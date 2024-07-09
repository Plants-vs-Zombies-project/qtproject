//设置昵称
function setName(name) {
    _content.friendModel.get(0).name=name;
}

//获取发送人昵称
function getName(){
    return _content.friendModel.get(0).name
}

//设置状态
function setStatus(status){
    _content.friendModel.get(0).status=status;
}

//设置图片
function setSource(source){
    _popI.source=source
}
//
