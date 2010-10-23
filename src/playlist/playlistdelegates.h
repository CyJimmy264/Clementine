/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLAYLISTDELEGATES_H
#define PLAYLISTDELEGATES_H

#include "playlist.h"
#include "library/library.h"

#include <QStyledItemDelegate>
#include <QTreeView>
#include <QStringListModel>
#include <QCompleter>

class QueuedItemDelegate : public QStyledItemDelegate {
public:
  QueuedItemDelegate(QObject* parent, int indicator_column = Playlist::Column_Title);
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  void DrawBox(QPainter* painter, const QRect& line_rect,
               const QFont& font, const QString& text, int width = -1) const;

  int queue_indicator_size(const QModelIndex& index) const;

private:
  static const int kQueueBoxBorder;
  static const int kQueueBoxCornerRadius;
  static const int kQueueBoxLength;
  static const QRgb kQueueBoxGradientColor1;
  static const QRgb kQueueBoxGradientColor2;
  static const int kQueueOpacitySteps;
  static const float kQueueOpacityLowerBound;

  int indicator_column_;
};

class PlaylistDelegateBase : public QueuedItemDelegate {
  Q_OBJECT
 public:
  PlaylistDelegateBase(QObject* parent, const QString& suffix = QString());
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  QString displayText(const QVariant& value, const QLocale& locale) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

  QStyleOptionViewItemV4 Adjusted(const QStyleOptionViewItem& option, const QModelIndex& index) const;

  static const int kMinHeight;

 public slots:
  bool helpEvent(QHelpEvent *event, QAbstractItemView *view,
                 const QStyleOptionViewItem &option, const QModelIndex &index);

 protected:
  QTreeView* view_;
  QString suffix_;
};

class LengthItemDelegate : public PlaylistDelegateBase {
 public:
  LengthItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

class SizeItemDelegate : public PlaylistDelegateBase {
 public:
  SizeItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

class DateItemDelegate : public PlaylistDelegateBase {
 public:
  DateItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

class LastPlayedItemDelegate : public PlaylistDelegateBase {
public:
  LastPlayedItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

class FileTypeItemDelegate : public PlaylistDelegateBase {
 public:
  FileTypeItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

class TextItemDelegate : public PlaylistDelegateBase {
 public:
  TextItemDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
      const QModelIndex& index) const;
};

class RatingItemDelegate : public PlaylistDelegateBase {
public:
  RatingItemDelegate(QObject* parent);
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QString displayText(const QVariant& value, const QLocale& locale) const;

  void set_mouse_over(const QModelIndex& index, const QPoint& pos) {
      mouse_over_index_ = index ; mouse_over_pos_ = pos; }
  void set_mouse_out() { mouse_over_index_ = QModelIndex(); }
  bool is_mouse_over() const { return mouse_over_index_.isValid(); }
  QModelIndex mouse_over_index() const { return mouse_over_index_; }

  static QRect ContentRect(const QRect& total);
  static double RatingForPos(const QPoint& pos, const QRect& total_rect);

  static const int kStarCount = 5;
  static const int kStarSize = 15;

private:
  QPixmap stars_[kStarCount*2+1];

  QModelIndex mouse_over_index_;
  QPoint mouse_over_pos_;
};

class TagCompletionModel : public QStringListModel {
 public:
  TagCompletionModel(LibraryBackend* backend, Playlist::Column column);
};

class TagCompleter : public QCompleter {
 public:
  TagCompleter(LibraryBackend* backend, Playlist::Column column, QLineEdit* editor);
};

class TagCompletionItemDelegate : public PlaylistDelegateBase {
 public:
  TagCompletionItemDelegate(QObject* parent, LibraryBackend* backend, Playlist::Column column) :
    PlaylistDelegateBase(parent), backend_(backend), column_(column) {};

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
      const QModelIndex& index) const;

 private:
  LibraryBackend* backend_;
  Playlist::Column column_;
};

class NativeSeparatorsDelegate : public PlaylistDelegateBase {
public:
  NativeSeparatorsDelegate(QObject* parent) : PlaylistDelegateBase(parent) {}
  QString displayText(const QVariant& value, const QLocale& locale) const;
};

#endif // PLAYLISTDELEGATES_H
