/*
    Copyright 2014 Ilya Zhuravlev

    This file is part of Acquisition.

    Acquisition is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Acquisition is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Acquisition.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QObject>
#include <QDateTime>
#include <string>

extern const std::string kShopTemplateItems;

class Application;

class Shop : public QObject {
    Q_OBJECT
public:
    explicit Shop(Application &app);
    void SetThread(const std::string &thread);
    void SetAutoUpdate(bool update);
    void SetShopTemplate(const std::string &shop_template);
    void Update();
    void CopyToClipboard();
    void ExpireShopData();
    void SubmitShopToForum();
    void SubmitShopBumpToForum();
    bool auto_update() const { return auto_update_; }
    const std::string &thread() const { return thread_; }
    const std::string &shop_data() const { return shop_data_; }
    const std::string &shop_template() const { return shop_template_; }
public slots:
    void OnEditPageFinished();
    void OnShopSubmitted();
    void OnBumpPageFinished();
    void OnBumpSubmitted();
private:
    std::string ShopEditUrl();
    std::string ShopBumpUrl();
    Application &app_;
    std::string thread_;
    std::string shop_data_;
    std::string shop_hash_;
    std::string shop_template_;
    bool shop_data_outdated_;
    bool auto_update_;

    QDateTime lastBumped_;
};
