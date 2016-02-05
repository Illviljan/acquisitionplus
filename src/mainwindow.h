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

#include <memory>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QItemSelection>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#ifdef Q_OS_WIN
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

#include "autoonline.h"
#include "bucket.h"
#include "items_model.h"
#include "porting.h"
#include "updatechecker.h"
#include "logchecker.h"
#include "forms/currencypane.h"


class QNetworkAccessManager;
class QNetworkReply;
class QVBoxLayout;

class Application;
class Column;
class Filter;
class FlowLayout;
class ImageCache;
class Search;

struct Buyout;

namespace Ui {
class MainWindow;
}

enum class TreeState {
    kExpand,
    kCollapse
};

enum class ProgramState {
    ItemsUpdating,
    ItemsReceive,
    ItemsPaused,
    ItemsCompleted,
    ShopSubmitting,
    ShopCompleted
};

struct CurrentStatusUpdate {
    ProgramState state;
    int progress, total;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(std::unique_ptr<Application> app);
    ~MainWindow();
    std::vector<Column*> columns;
    void RemoveTab(int index);
    void GenerateCurrentItemHeader(const std::shared_ptr<Item> &item);
    void UpdateSettingsBox();
    void InitializeActions();
    Application* application() { return app_.get(); }
    void LoadSearches();
    void SaveSearches();
public slots:
    void OnTreeChange(const QItemSelection &selected, const QItemSelection &deselected);
    void OnSearchFormChange();
    void OnTabChange(int index);
    void OnImageFetched(QNetworkReply *reply);
    void OnItemsRefreshed();
    void OnStatusUpdate(const CurrentStatusUpdate &status);
    void OnBuyoutChange(bool doParse=true);
    void OnExpandAll();
    void OnCollapseAll();
    void OnUpdateAvailable();
    void OnOnlineUpdate(bool online);
    void OnTabClose(int index);
    void ToggleBucketAtMenu();
    void ToggleShowHiddenBuckets(bool checked);
protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);
private slots:
    void on_advancedSearchButton_toggled(bool checked);
    void on_refreshItemsButton_clicked();
    void on_updateShopButton_clicked();

private:
    void UpdateCurrentHeaderState();
    void UpdateCurrentSelection();
    void UpdateCurrentBucket(const Bucket &bucket);
    void UpdateCurrentItem(const std::shared_ptr<Item> &item);
    void UpdateCurrentItemMinimap(const std::shared_ptr<Item> &item);
    void UpdateCurrentItemIcon(const std::shared_ptr<Item> &item, const QImage &image);
    void UpdateCurrentItemProperties(const std::shared_ptr<Item> &item);
    void UpdateCurrentBuyout();
    Search *NewSearch();
    void InitializeLogging();
    void InitializeSearchForm();
    void InitializeUi();
    void AddSearchGroup(QLayout *layout, const std::string &name);
    bool eventFilter(QObject *o, QEvent *e);
    void ResetBuyoutWidgets();
    void UpdateBuyoutWidgets(const Buyout &bo);
    void ExpandCollapse(TreeState state);
    void UpdateOnlineGui();

    std::unique_ptr<Application> app_;
    Ui::MainWindow *ui;
    Items current_items_;
    QList<Bucket> current_buckets_;
    std::vector<Search*> searches_;
    Search *current_search_;
    QTabBar *tab_bar_;
    std::vector<std::unique_ptr<Filter>> filters_;
    int search_count_;
    QNetworkAccessManager *image_network_manager_;
    ImageCache *image_cache_;
    QLabel *status_bar_label_;
    QVBoxLayout *search_form_layout_;
    QMenu tab_context_menu_;
    QMenu default_context_menu_;
    QAction* default_context_menu_showhidden_;
    QMenu bucket_context_menu_;
    QAction* bucket_context_menu_toggle_;
    QMenu view_header_menu_;
    UpdateChecker update_checker_;
    QPushButton update_button_;
    AutoOnline auto_online_;
    QLabel online_label_;
#ifdef Q_OS_WIN32
    QWinTaskbarButton *taskbar_button_;
#endif
    QSystemTrayIcon tray_icon_;
    LogChecker log_checker_;

    QMap<int, QAction*> view_header_actions_;

    // Maybe not a good thing?
    friend class SettingsPane;
};
