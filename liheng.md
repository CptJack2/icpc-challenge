#问题
1、对于广告RTB业务模式和服务场景的扩展了解
2、针对一些典型技术场景和难点或者特定场景功能的技术方案设计和方案
a. 针对广告对竞价广告的可配置式的频次控制。例如：控制某广告1天内针对每个用户最多3次展示
b. 实现多节点多实例中，整体控制广告预算的消耗，避免超预算花费（PS：部分广告延迟曝光可能导致真实消费延迟达到）
c. 针对大量设备ID的标签数据的存储结构设计

方案尽量可能包含但不限于：方案思路设计、伪代码等

#广告RTB
简介视频
How Does Real-Time Bidding Work? | Explained in Under 5 Minutes https://www.youtube.com/watch?v=ylhKJSrxutM
How an Ad is Served with Real Time Bidding (RTB) - IAB Digital Simplified https://www.youtube.com/watch?v=-Glgi9RRuJs

SSP(supply side platform) 内容平台,投放广告的场所,提供浏览广告的用户
AdEx(advertisement exchange) 促成SSP和DSP进行交易的交易所,力恒属于这种类型的参与者.
DSP(demand side platform) 广告主,将广告投放到其他平台

#对于广告RTB业务模式和服务场景的扩展了解
RTB(Real Time Binding),是一种互联网广告投放的模式, 其他的主流互联网广告投放模式包括CPM(cost per mille,每千次浏览付费),CPC(cost per click,每次点击付费),Monthly/Daily Flat(包月包天).
RTB的主要过程如下,用户先登入SSP的网站(APP),SSP对此用户产生一个广告供应,将这个卖单连同用户画像(年龄,性别,兴趣,职业等)在AdEx下卖单,同时DSP也在AdEx对广告投放的目标客户(指定的年龄,性别,兴趣等特征)下买单,由AdEx作为交易所实时撮合成交.成交后,SSP直接从DSP处获取广告物料发送到用户处,广告就直接展示在了用户的终端上.
整个过程的时间是很短的,近似实时,在SSP的服务器收到用户的请求时就会同时向AdEx下单,完成交易后,SSP会把DSP的广告连同自己的内容一起响应给用户(或者下发广告链接客户端自行请求),整个过程是百毫秒级的.