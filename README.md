# HelloVanGogh

IPSE的梵高计划：

只需要用户上传一张图片，利用循环神经网络CycleGAN的AI技术，将图片转换成印象派风格的艺术照片，采用人工手绘和机器喷绘制作成精美的油画作品。用户购买后邮寄到用户家里。

![](https://camo.githubusercontent.com/1862adecd202ba420847653d9a119f9fed9a3abd/68747470733a2f2f6a756e79616e7a2e6769746875622e696f2f4379636c6547414e2f696d616765732f70686f746f327061696e74696e672e6a7067)

结合IPSE的珍贵信息上链技术，用户可以选择不同POST数量的寄语，这些珍贵的POST寄语将永久上链保存。用户可以扫描二维码读取到这些珍贵的寄语信息。同时POST还具有极高的收藏价值。

上面将人工智能、艺术、制造业、区块链等不同技术组合运用的案例，其实可以总结背后的核心：技术要落地。IPSE将结合运用各种技术将区块链落地的征途中，永远不惧怕被嘲笑。当然梵高计划绝不是简简单单的艺术风格绘画作品制作而已，而是将区块链技术和实体产业、实体经济的努力尝试，因为接下来介绍的投资者参与和退出都会设计到公司引入区块链技术后的新治理模式。

初略来说，传统股权投资规模大，参与性差，结合区块链的通证经济模型，让股权投资参与门槛降低，而且紧密结合社区运营，让产品能够更加快速做出口碑，拥有更加广泛的市场背书群体。IPSE的梵高计划，其实就是这样一个小小的尝试。

梵高计划是IPSE努力将POST通证价值落地的子项目，赋予POST更多价值是IPSE团队一直努力追求的方向。

### 投资者参与

梵高计划将成立一家新的公司，专门负责核算此项目经营性收入，任何人都可以使用POST来参与公司的股权投资，且比例和总数额都是通过智能合约的方式上链。公司将每个季度核算营收给股东分红。参与者越早参与项目，获得的股份比例就越高，所投POST数量和所占股份比例遵循下面的公式：

	股份占比 = 3.414 / (m % 10000000 + 1) * t / n
	
> m为所有投资者所投POST总额
> 
> t为该用户投POST数额
> 
> n为梵高计划所募总额，目前计划1一个亿POST
> 
> 也就说，在前10%的投资中，将获得3.414倍的股份比例。在10%到20%的投资中，将获得1.707倍的股份比例，依次列推。

### 投资者退出

任何投资者在任何时候都可以退出，只是要将所投POST除以3.414取出即可。这样投资者在牺牲掉一些利益后，得到了投资进退的自由，其他投资者的利益也将得到保护。

### 商品价值

除了使用了AI的技术，赋予照片艺术风格外，最为重要的是定制生产，永恒的意义将通过区块链来记录。会有5种不同规格可选，分别对应1000POST，1万POST，10万POST，100万POST，1000万POST，代表所赠送的艺术画作为礼品的价值。不同规格的作品定价权将是新公司独有。

### 技术细节

- 合约表 投资者表 investtable

invite表：投资者账号，投资额，创建时间，状态，额外信息

- 全局变量表 globalvar

全局变量表：目前投资总余额，历史投资总额，已转出数额，状态

- 方法1：投资invite

用户转账到hellovangogh，累加后，目前投资总余额未超过1亿POST，则可以记录为合规投资，根据上面公式记录其投资额。

> 比如，其在一开始就参与投资，投了100万POST，投资表中会记录该用户投资了341.4万POST，占股比为3.414%。
> 
> 如果筹资未达到1亿POST，在第一个季度结算前，公司自己将填补最后的缺口。


- 方法2:退出投资quitinvite

用户签名，提取相应数额POST的投资，合约账号将按照比例减去投资额，并且给该用户转账，累加已转出数额。

- 方法3:给购买艺术画作品加入POST selltoken

合约执行该方法，累加已转出数额，目前投资总余额减去给作品添加的POST数额。

- DAPP读取

DAPP读取hellovangogh给其他账号的转账，排除掉退出投资的记录，就是购买艺术画作品时上链的记录，需要将转账信息做一个精美的卡片，链接到二维码，扫码即可查看。主要信息记录在memo信息中。


### 图片欣赏

> 巴厘岛的早晨

![](https://upload.cc/i1/2019/08/08/g2vF0m.jpeg)
![](https://upload.cc/i1/2019/08/08/n9wKZv.jpeg)

	上面梵高的绘画风格，用来处理现代花园也能有独特的味道。对于流动的池水的风格转换，很讨人喜欢。
	下面塞尚的绘画风格，塞尚作为后印象派的实力大师，物象的结实感和画面的深度被着重体现出来，应该会有不少人喜欢这种风格。
	
> 蓝梦岛上的新娘

![](https://upload.cc/i1/2019/08/08/07dD6x.jpeg)

	莫奈的绘画风格，不见执伞的女子和幽静的荷塘小池，莫奈风格用来处理人物照也是朦胧美和整体感非常好。
	
> 
