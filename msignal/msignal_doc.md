信号

接收元Mzcy::Receiver<Y,Ym>
发送元Mzcy::Sender<Ym>
其中Ym是信息类型, Y是接收者类型
接收元需要作为接收者的一个公开成员
发送元需要作为发送者的一个公开成员
接收元和发送元必须使用同样的信息类型Ym, 才能进行传递

sender.exec(message)
对于发送者, 需要在自己的方法内部主动调用发送元的sender.exec(message)方法

receiver.init(X,Y)
对于接收者, 需要初始化接收元receiver.init(X,Y),
把接收者指针X和接收者的指定的公开方法指针Y传给接收元,
此后receiver.exec(message)会自动地正确调用接收者的指定的公开方法,
否则无效但没有错误


发送元会维持一个接收元记录列表, 可以查询自己的状态
sender.num()             // 查询接收元记录的个数
sender.is_bind(receiver) // 查询接收元参数是否已被记录

接收元维持一个发送元记录列表, 可以查询自己的状态
receiver.num()           // 查询发送元记录的个数
receiver.is_bind(sender) // 查询发送元参数是否已被记录
receiver.is_init()       // 查询现在是否已经初始化


绑定与解绑
sender.bind(receiver)    // 发送元可以绑定接收元
sender.unbind(receiver)  // 发送元可以解绑接收元
sender.unbind_all()      // 发送元可以解绑所有的接收元, 在析构时会自动调用

与QT的信号槽函数不同, 发送元和接收元在地位上是等价的,
它们可以等效地向对方发起绑定 receiver.bind(sender)    // 接收元可以绑定发送元
receiver.unbind(sender)  // 接收元可以解绑发送元
receiver.unbind_all()    // 接收元可以解绑所有的发送元, 在析构时会自动调用


sender.name(name_str)
receiver.name(name_str)
可以给发送元和接收元指定一个const char*类型的名称字符串name_str
如果设置了, 则在调用exec()时会首先把名称字符串传递给cout,
从而让信号传递过程可视化 可以传入一个nullptr用来去除名称字符串

不要使用_detail_开头的接口, 这是不合适的操作, 但又不方便设置私有权限,
可能导致指针和析构的错误
