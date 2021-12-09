#include "axml.h"
// https://www.twblogs.net/a/5edbd54d3b859a4f024b4ffd
/*
Qt的Xml操作QDomDocument
Qt對於Xml的支持是很好的，一些我們需要的操作應有盡有，下面簡單介紹一下怎樣使用。主要有以下幾點使用：

寫xml到文件
讀xml
添加節點到xml
刪除xml中某節點信息
修改xml中某節點信息
準備工作
.pro加入QT += xml
需要include QDomDocument QTextStream QFile三個頭文件
*/
AXml::AXml()
{

}

void AXml::writeXML()
{
    //打開或創建文件
    QString fileName{"test.xml"};
    QFile file(fileName);
    //QIODevice::Truncate表示清空原來的內容
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;

    //建立 xml 文檔在內存中
    QDomDocument doc;
    //添加處理命令
    QDomProcessingInstruction instruction;
    QString version = "version=\"1.0\"";
    QString encoding = " encoding=\"UTF-8\"";
    instruction = doc.createProcessingInstruction("xml", version + encoding);
    //instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    //創建註釋
    QDomComment comment;
    comment = doc.createComment(QString::fromLocal8Bit("VPG-2238 test XML"));
    QDomProcessingInstruction styleInstruction;
    styleInstruction= doc.createProcessingInstruction("xml-stylesheet", "type=\"text/css\" href=\"style.css\"");
    doc.appendChild(instruction); //文檔開始聲明
    doc.appendChild(comment);
    doc.appendChild(styleInstruction);  // 處理指令


    //添加根節點
    QDomElement root=doc.createElement("library");
    root.setAttribute("Version","2.1");
    doc.appendChild(root);
    //添加第一個子節點及其子元素
    QDomElement book=doc.createElement("book");
    //方式一：創建屬性  其中鍵值對的值可以是各種類型
    book.setAttribute("id",1);
    //方式二：創建屬性 值必須是字符串
    QDomAttr time=doc.createAttribute("time");
    time.setValue("2013/6/13");
    book.setAttributeNode(time);
    QDomElement title=doc.createElement("title"); //創建子元素
    QDomText text; //設置括號標籤中間的值
    text=doc.createTextNode("C++ primer");
    book.appendChild(title);
    title.appendChild(text);
    QDomElement author=doc.createElement("author"); //創建子元素
    text=doc.createTextNode("Stanley Lippman");
    author.appendChild(text);
    book.appendChild(author);
    //添加節點book做爲根節點的子節點
    root.appendChild(book);

    //添加第二個子節點及其子元素
    book=doc.createElement("book");
    book.setAttribute("id",2);
    time=doc.createAttribute("time");
    time.setValue("2007/5/25");
    book.setAttributeNode(time);
    title=doc.createElement("title");
    text=doc.createTextNode("Thinking in Java");
    book.appendChild(title);
    title.appendChild(text);

    author=doc.createElement("author");
    text=doc.createTextNode("Bruce Eckel");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    //輸出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //縮進4格
    file.close();
}

void AXml::readXML()
{
    //打開或創建文件
        QFile file("test.xml");
        if(!file.open(QFile::ReadOnly)) {
            qDebug() << "Err:1 , open file Fail...";
            return;
        }


        QDomDocument doc;
        //設置wangfei1.xml到文檔
        if(!doc.setContent(&file))
        {
            qDebug() << "Err: 2";
            file.close();
            return;
        }

        file.close();

        //返回根節點
        QDomElement root=doc.documentElement();
        qDebug()<<root.nodeName();
        //如果xml根元素有屬性（Version）將輸出，Vinsion是用戶自定義的屬性，沒有繼續執行下一條命令
        if (root.hasAttribute("Version"))  // 屬性
            qDebug() << root.attribute("Version");
        /**********根元素之上（XML 聲明、註釋等）**********/
        QDomNode node = root.previousSibling();
        while (!node.isNull())
        {
            switch (node.nodeType())
            {
            case QDomNode::ProcessingInstructionNode :
            {
                QDomProcessingInstruction instruction = node.toProcessingInstruction();
                //輸出處理指令，是用戶自定義的，比如字符串“name”對應處理指令得到名字，這個命令是用戶寫的
                qDebug() << instruction.target() << instruction.data();
                if (QString::compare(instruction.target(), "xml") == 0) // 開始文檔（XML 聲明）
                {
                    //cout<<"處理命令xml"<<endl;
                    // ...
                }
                else if (QString::compare(instruction.target(), "xml-stylesheet") == 0) // 處理指令
                {
                    //cout<<"處理命令xml-stylesheet"<<endl;
                    // ...
                }
                break;
            }
            case QDomNode::CommentNode :
            {
                QDomComment comment = node.toComment();
                qDebug() << comment.data();
                break;
            }
            default:
                break;
            }
            node = node.previousSibling();
        }

        //獲得第一個子節點
        node=root.firstChild();
        while(!node.isNull())  //如果節點不空
        {
            if(node.isElement()) //如果節點是元素
            {
                //轉換爲元素
                QDomElement element=node.toElement();
                if (!element.isNull())// 節點的確是一個元素
                {
                    //輸出第一個節點，包括第一個節點的屬性，這個屬性需要指定屬性值，才能輸出，如果沒有輸出空
                    qDebug()<<element.tagName()<<" "<<element.attribute("id")<<" "<<element.attribute("time");
                    QDomNodeList list=element.childNodes();
                    for(int i=0;i<list.count();++i)
                    {
                        QDomNode n=list.at(i);
                        //node = list.at(i);
                        if(node.isElement())
                        {
                            qDebug()<<n.nodeName()<<":"<<n.toElement().text();
                            element = n.toElement();
                            //qDebug()<<element.nodeName()<<":"<<element.toElement().text();
                            if (QString::compare(element.tagName(), QStringLiteral("作者")) == 0)
                            {
                                // ...處理命令
                                //cout<< "處理命令作者"<<endl;
                            }
                            else if (QString::compare(element.tagName(), QStringLiteral("時間")) == 0)
                            {
                                //cout<<"處理命令時間"<<endl;
                                // ...處理命令
                            }
                            else if (QString::compare(element.tagName(), QStringLiteral("個人說明")) == 0)
                            {
                                //cout<<"處理命令個人說明"<<endl;
                                // ...處理命令
                            }
                        }

                    }
                }
            }
            //下一個兄弟節點
            node=node.nextSibling();
        }
}
void AXml::addXML()
{
    //打開文件
    QFile file("test.xml");
    if(!file.open(QFile::ReadOnly))
        return;
    //增加一個一級子節點以及元素
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    //創建根節點
    QDomElement root = doc.documentElement();
    //創建next子節點book
    QDomElement book = doc.createElement("book");
    book.setAttribute("id",3);
    book.setAttribute("time","1813/1/27");
    QDomElement title = doc.createElement("title");
    QDomText text;
    text = doc.createTextNode("Pride and Prejudice");
    //添加text內容到title節點
    title.appendChild(text);
    //添加title到book節點
    book.appendChild(title);
    //添加book到根節點
    root.appendChild(book);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //輸出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //以縮進4格方式輸出，也有其他輸出方式（格式）
    file.close();
}
void AXml::deleteXML()
{
    //打開文件
    QFile file("test.xml");
    if(!file.open(QFile::ReadOnly))
        return;

    //刪除一個一級子節點及其元素，外層節點刪除內層節點於此相同
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    //由標籤名定位，本標籤爲book，以後可以是用string類型的字符串替換，實現動態
    QDomNodeList list=doc.elementsByTagName("book");

    //刪除方式一，通過標籤book後面的屬性刪除<book>到</book>
    for(int i=0;i<list.count();i++)
    {
        //轉化爲元素
        QDomElement e=list.at(i).toElement();
        //找到time是2007/5/25這一條數據將其刪除
        if(e.attribute("time")=="2007/5/25")
            root.removeChild(list.at(i));
    }
    //刪除方式二，可以通過索引直接刪除
//    root.removeChild(list.at(1));

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //輸出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //縮進4格
    file.close();
}
void AXml::amendXML()
{
    //打開文件
    QFile file("wangfei1.xml");
    if(!file.open(QFile::ReadOnly))
        return;

    //更新一個標籤項,如果知道xml的結構，直接定位到那個標籤上定點更新
    //或者用遍歷的方法去匹配tagname或者attribut，value來更新
    QDomDocument doc;
    //設置wangfei.xml讀到doc文檔
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    /**
     *    知道xml結構，直接定位修改
      *   提取根節點
      *
      */
    QDomElement root=doc.documentElement();
    //以book標籤存入list
//    QDomNodeList list=root.elementsByTagName("book");
//    QDomNode listNode=list.at(list.size()-2).firstChild();
//    QDomNode oldnode=listNode.firstChild();
//    //把title改成Emma
//    listNode.firstChild().setNodeValue("aaaaaaa");
//    QDomNode newnode=listNode.firstChild();
//    listNode.replaceChild(newnode,oldnode);


    //使用遍歷方法選擇要修改的元素
//    QDomNodeList lists =doc.childNodes();
//    QDomNodeList list=root.elementsByTagName("book");

    QDomNode node = root.firstChild();

    //QDomNodeList list=root.elementsByTagName("book");

    while(!node.isNull())  //如果節點不空
    {
        if(node.isElement()) //如果節點是元素
        {
            //轉換爲元素
            QDomElement element=node.toElement();
            if (!element.isNull() && element.attribute("id") == "3")// 節點的確是一個元素
            {
                //輸出第一個節點，包括第一個節點的屬性，這個屬性需要指定屬性值，才能輸出，如果沒有輸出空
                //qDebug()<<element.tagName()<<" "<<element.attribute("id")<<" "<<element.attribute("time");
                QDomNodeList list=element.childNodes();
                for(int i=0;i<list.count();++i)
                {
                    QDomNode n=list.at(i);
                    //node = list.at(i);
                    if(node.isElement())
                    {
                        //qDebug()<<n.nodeName()<<":"<<n.toElement().text();
                        element = n.toElement();
                        //與上面qDebug效果相同
                        //qDebug()<<element.nodeName()<<":"<<element.toElement().text();


                        //這個if可以不需要，如果需要精確定位，以防數據相同所以要加這個嵌套if
                        if (QString::compare(element.tagName(), QStringLiteral("title")) == 0)
                        {

                            if("Pride and Prejudice" == element.toElement().text())
                            {
                                // ...處理命令，在這個if裏可以遍歷想要的節點進行修改
                                //新建一箇舊的node緩存
                                QDomNode oldNode = n.firstChild();
                                n.firstChild().setNodeValue("changchun1");
                                //新建一個新的newNode子節點
                                QDomNode newNode = n.firstChild();
                                //使用replaceChild替換node
                                n.replaceChild(newNode,oldNode);
                            }

                        }

                        else if (QString::compare(element.tagName(), QStringLiteral("時間")) == 0)
                        {
                            //cout<<"處理命令時間"<<endl;
                            // ...處理命令
                        }
                        else if (QString::compare(element.tagName(), QStringLiteral("個人說明")) == 0)
                        {
                            //cout<<"處理命令個人說明"<<endl;
                            // ...處理命令
                        }
                    }

                }
            }
        }
        //下一個兄弟節點
        node=node.nextSibling();
    }


    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //輸出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //縮進4格
    file.close();

}
