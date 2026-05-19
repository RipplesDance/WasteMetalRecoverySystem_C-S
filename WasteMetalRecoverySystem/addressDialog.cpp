#include "addressDialog.h"
#include "ui_addressDialog.h"

addressDialog::addressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addressDialog)
{
    ui->setupUi(this);
    //post address
    post_address.fullName = "林风";
    post_address.phoneNumber = "15503829181";
    post_address.province = "上海市";
    post_address.city = "上海市";
    post_address.district = "松江区";
    post_address.detail = "XX有限公司";

    // update cities when province changes
    connect(ui->province_comboBox, &QComboBox::currentTextChanged, [=](const QString &province){
        ui->city_comboBox->clear();
        auto cities = pcd_pair.value(province);

        if (cities.keys().contains("市辖区") && cities.size() == 1) {
            ui->city_comboBox->addItem(province);
        } else {
            ui->city_comboBox->addItems(cities.keys());
        }
    });

    // update district when cities changes
    connect(ui->city_comboBox, &QComboBox::currentTextChanged, [=](const QString &cityText){
        ui->district_comboBox->clear();
        QString province = ui->province_comboBox->currentText();
        auto cities = pcd_pair.value(province);

        if (cityText == province && cities.contains("市辖区")) {
            ui->district_comboBox->addItems(cities.value("市辖区"));
        } else {
            ui->district_comboBox->addItems(cities.value(cityText));
        }
    });

    //connect button events
    connect(ui->save_btn, &QPushButton::clicked, this, &addressDialog::save_btn_clicked);

    m_contentLayout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    //防御性检查：如果获取不到（或者获取到的不是 QVBoxLayout），则手动创建
    if (!m_contentLayout) {
        m_contentLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        // 手动关联布局到内容窗口
        ui->scrollAreaWidgetContents->setLayout(m_contentLayout);
    }
    m_contentLayout->addStretch();

    ui->phoneLineEdit->setInputMask("999-9999-9999"); // 强制格式

    parseAddressJson();
    fetchLocationByIP();
    init();
}

addressDialog::~addressDialog()
{
    delete ui;
}

void addressDialog::init()
{
    addressList.clear();
    addressList = address().readAllAddressFromLocal();

    for(address data : addressList)
        qDebug()<<"init的data" << data;

    refreshUi();
}

void addressDialog::setAddress(address data)
{
    QString fullName = data.fullName;
    QString phoneNumber = data.phoneNumber;
    QString province = data.province;
    QString city = data.city;
    QString district = data.district;
    QString detail = data.detail;
    QString zipCode = data.zipCode;
    bool isDefault = data.isDefault;

    ui->nameLineEdit->setText(fullName);
    ui->phoneLineEdit->setText(phoneNumber);
    ui->city_comboBox->setEditText(city);
    ui->province_comboBox->setEditText(province);
    ui->district_comboBox->setEditText(district);
    ui->detailAddrTextEdit->setText(detail);
    ui->zipCodeLineEdit->setText(zipCode);
    if(isDefault)
        ui->defaultCheckBox->setCheckState(Qt::CheckState::Checked);
    else
        ui->defaultCheckBox->setCheckState(Qt::CheckState::Unchecked);
}

void addressDialog::putAddressToUi(address data)
{
    QString fullName = data.fullName;
    QString phoneNumber = data.phoneNumber;
    QString province = data.province;
    QString city = data.city;
    QString district = data.district;
    QString detail = data.detail;
    QString zipCode = data.zipCode;
    bool isDefault = data.isDefault;

    data.detail=data.detail.trimmed();
    data.phoneNumber = data.phoneNumber.trimmed();
    data.fullName = data.fullName.trimmed();
    // 2. 实例化自定义的交互卡片
    interactableFrame *card = new interactableFrame(ui->scrollAreaWidgetContents);
    card->setMinimumHeight(100);

    // 3. 为卡片构建内部布局并填充信息
    // 建议在 InteractableFrame 内部封装一个 setData 函数，或者直接在此构建
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(15, 10, 15, 10);
    cardLayout->setSpacing(4);

    // 第一行：姓名 + 电话 (加粗)
    QLabel *namePhoneLabel = new QLabel(QString("%1  %2").arg(data.fullName, data.phoneNumber));
    namePhoneLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #263238;");

    // 第二行：省市区
    QLabel *regionLabel = new QLabel(QString("%1 %2 %3").arg(data.province, data.city, data.district));
    regionLabel->setStyleSheet("font-size: 18px; color: #546E7A;");

    // 第三行：详细地址 (自动换行)
    QLabel *detailLabel = new QLabel(data.detail);
    detailLabel->setWordWrap(true);
    detailLabel->setStyleSheet("font-size: 18px; color: #263238;");

    cardLayout->addWidget(namePhoneLabel);
    cardLayout->addWidget(regionLabel);
    cardLayout->addWidget(detailLabel);

    //添加默认标签
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch(); // 弹簧把标签推向右边
    if (isDefault) {
        QLabel *defaultTag = new QLabel("默认");
        // 设置样式：长方形、背景色、居中、固定宽度（实现你想要的比例感）
        defaultTag->setStyleSheet(
                    "QLabel {"
                    "  color: #4CAF50;"
                    "  border: none;"
                    "  font-size: 24px;"
                    "  font-weight: bold"
                    "}"
                    );
        defaultTag->setAlignment(Qt::AlignCenter);
        defaultTag->setFixedWidth(this->width() / 2); // 宽度设为卡片的一半

        // 确保标签也不拦截点击，透传给卡片本身
        defaultTag->setAttribute(Qt::WA_TransparentForMouseEvents);
        defaultTag->setBackgroundRole(QPalette::NoRole);

        bottomLayout->addWidget(defaultTag);
        cardLayout->addLayout(bottomLayout);
    }

    // 4. 执行“点击穿透”逻辑
    QList<QLabel*> labels = card->findChildren<QLabel*>();
    for (QLabel* lbl : labels) {
        lbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        lbl->setBackgroundRole(QPalette::NoRole); // 确保背景透明
    }
    // 5. 插入到 ScrollArea 的布局中
    int insertIndex = qMax(0, m_contentLayout->count() - 1);
    m_contentLayout->insertWidget(insertIndex, card);

    // 6. 连接点击信号
    connect(card, &interactableFrame::clicked, this, [=](){
        QString fullAddress = address().getFullAddress(&data);
        currentEditingId = data.id;
        setAddress(data);
    });

    // 7. 处理右键菜单 (新增部分)
        // 设置策略为自定义上下文菜单
        card->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(card, &interactableFrame::customContextMenuRequested, this, [=](const QPoint &pos){
            QMenu menu(this);

            // 样式美化（可选）
            menu.setStyleSheet("QMenu { background-color: white; border: 1px solid #dcdcdc; }"
                               "QMenu::item { padding: 8px 25px; }"
                               "QMenu::item:selected { background-color: #e8f5e9; color: #2e7d32; }");

            QAction *setDefaultAct = new QAction("设为默认", &menu);
            QAction *deleteAct = new QAction("删除地址", &menu);

            // 如果已经是默认地址，可以禁用该项
            if(data.isDefault) setDefaultAct->setEnabled(false);

            // --- 逻辑：设为默认 ---
            connect(setDefaultAct, &QAction::triggered, this, [=](){
                // 1. 将所有地址设为非默认
                for(int i=0; i < addressList.size(); ++i) {
                    addressList[i].isDefault = false;
                    addressList[i].saveAddressToLocal(addressList[i]);
                }
                // 2. 设置当前地址为默认并保存
                address updatedData = data;
                updatedData.isDefault = true;
                updatedData.saveAddressToLocal(updatedData);

                init(); // 重新加载数据并刷新UI
            });

            // --- 逻辑：删除 ---
            connect(deleteAct, &QAction::triggered, this, [=](){
                auto reply = QMessageBox::question(this, "确认", "确定要删除该地址吗？");
                if(reply == QMessageBox::Yes) {
                    address().removeFromLocal(data);
                    init(); // 重新加载数据并刷新UI
                }
            });

            menu.addAction(setDefaultAct);
            menu.addSeparator();
            menu.addAction(deleteAct);

            // 在鼠标当前位置弹出
            menu.exec(card->mapToGlobal(pos));
        });

}

void addressDialog::refreshUi() {
    // 1. 清空布局
    QLayoutItem *child;
    while ((child = m_contentLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater(); // 延迟删除子控件
        }
        delete child; // 删除布局项本身
    }

    // 2. 补回弹簧（非常重要，否则卡片会垂直居中散开）
    m_contentLayout->addStretch();

    // 3. 重新填入
    for (const address &addr : addressList) {
        putAddressToUi(addr);
    }
}

void addressDialog::fetchLocationByIP() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://ip-api.com/json/?lang=zh-CN");

    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    // 连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug()<<"parsing";
            parseLocationJson(reply->readAll());
        } else {
            qDebug() << "IP定位失败:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void addressDialog::parseLocationJson(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (obj["status"].toString() == "success") {
        QString province = obj["regionName"].toString(); // 示例: "广东省"
        QString city = obj["city"].toString();           // 示例: "深圳市"

        qDebug()<<province << city;
        // 自动设置 ComboBox
        autoSelectRegion(province, city);
    }
}

void addressDialog::autoSelectRegion(const QString &province, const QString &city) {
    // 1. 设置省份
    int pIndex = ui->province_comboBox->findText(province, Qt::MatchContains);
    if (pIndex != -1) {
        ui->province_comboBox->setCurrentIndex(pIndex);

        // 2. 设置城市 (省份改变后，cityCombo 已由信号槽自动更新)
        // 建议增加一个微小延迟，确保 cityCombo 已经填充完成
        QTimer::singleShot(100, this, [=](){
            int cIndex = ui->city_comboBox->findText(city, Qt::MatchContains);
            if (cIndex != -1) {
                ui->city_comboBox->setCurrentIndex(cIndex);
            }
        });
    }
}

void addressDialog::parseAddressJson()
{
    QFile file(":/json/json.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject rootObj = doc.object();

    // 遍历省份/直辖市
    for (auto it = rootObj.begin(); it != rootObj.end(); ++it) {
        QString provinceName = it.key();
        QJsonObject citiesObj = it.value().toObject();

        QMap<QString, QStringList> cityMap;
        for (auto cityIt = citiesObj.begin(); cityIt != citiesObj.end(); ++cityIt) {
            QString cityName = cityIt.key();
            QStringList districts;
            QJsonArray distArr = cityIt.value().toArray();
            for (const QJsonValue &v : distArr) districts << v.toString();

            cityMap.insert(cityName, districts);
        }
        pcd_pair.insert(provinceName, cityMap);
    }

    // 初始化省份下拉框
    ui->province_comboBox->addItems(pcd_pair.keys());
}

void addressDialog::clearUi()
{
    ui->nameLineEdit->clear();
    ui->phoneLineEdit->clear();
    ui->detailAddrTextEdit->clear();
    ui->zipCodeLineEdit->clear();
    currentEditingId = "";
}

address addressDialog::getDefaultAddress()
{
    address value;
    for(address data : addressList)
    {
        if(data.isDefault == true)
        {
            value = data;
            break;
        }
    }
    return value;
}

bool addressDialog::isDefaultExists()
{
    for(address add : addressList)
    {
        if(add.isDefault == true)
            return true;
    }
    return false;
}

void addressDialog::save_btn_clicked()
{
    // 1. 提取 UI 上的所有原始数据
    address data;
    data.id = data.generateId();
    qDebug()<<"save_btn_clicked";
    data.fullName = ui->nameLineEdit->text();
    data.phoneNumber = ui->phoneLineEdit->text();
    data.province = ui->province_comboBox->currentText();
    data.city = ui->city_comboBox->currentText();
    data.district = ui->district_comboBox->currentText();
    data.zipCode = ui->zipCodeLineEdit->text();
    data.detail = ui->detailAddrTextEdit->toPlainText(); // 使用 QTextEdit 获取详细地址
    data.isDefault = ui->defaultCheckBox->checkState();

    if (data.fullName.isEmpty() || data.phoneNumber.isEmpty() || data.province.isEmpty() || !data.isValid()) {
        QMessageBox::warning(this,"警告","必填项为空或不符合规范！");
        return;
    }

//    if(data.isDefault == true && isDefaultExists())
//    {
//        QMessageBox::warning(this,"警告","默认地址只能存在一个！");
//        return;
//    }

    if (data.isDefault) {
            // 将本地所有其他地址的 isDefault 设为 false
            QList<address> all = address().readAllAddressFromLocal();
            for (address &oldAddr : all) {
                if (oldAddr.id != data.id && oldAddr.isDefault) {
                    oldAddr.isDefault = false;
                    oldAddr.saveAddressToLocal(oldAddr); // 覆盖旧文件
                }
            }
        }

    if(data.id.isEmpty())
        return;
    data.saveAddressToLocal(data);
    clearUi();
    init();
}



