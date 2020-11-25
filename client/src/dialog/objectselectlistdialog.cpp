/**
 * client/src/dialog/objectselectlistdialog.cpp
 *
 * This file is part of the traintastic source code.
 *
 * Copyright (C) 2019-2020 Reinder Feenstra
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "objectselectlistdialog.hpp"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtWaitingSpinner/waitingspinnerwidget.h>
#include "../network/connection.hpp"
#include "../network/object.hpp"
#include "../network/utils.hpp"
#include "../widget/tablewidget.hpp"
#include "../widget/alertwidget.hpp"
#include <traintastic/locale/locale.hpp>

ObjectSelectListDialog::ObjectSelectListDialog(ObjectProperty& property, QWidget* parent) :
  QDialog(parent, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
  m_property{property},
  m_buttons{new QDialogButtonBox(this)},
  m_tableWidget{new TableWidget()}
{
  setWindowTitle(Locale::tr("qtapp.object_select_list_dialog:select_object"));

  m_buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  m_buttons->button(QDialogButtonBox::Ok)->setText(Locale::tr("qtapp.object_select_list_dialog:ok"));
  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
  connect(m_buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this,
    [this]()
    {
      m_property.setByObjectId(m_tableWidget->getRowObjectId(m_tableWidget->selectionModel()->selectedIndexes().first().row()));
      accept();
    });
  m_buttons->button(QDialogButtonBox::Cancel)->setText(Locale::tr("qtapp.object_select_list_dialog:cancel"));
  connect(m_buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ObjectSelectListDialog::reject);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setMargin(0);
  layout->addWidget(m_tableWidget);
  layout->addWidget(m_buttons);
  //layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  auto* spinner = new WaitingSpinnerWidget(this, true, false);
  spinner->start();

  m_requestId = m_property.object().connection()->getObject(m_property.getAttribute(AttributeName::ObjectList, QVariant()).toString(),
    [this, spinner](const ObjectPtr& object, Message::ErrorCode ec)
    {
      if(object)
      {
        m_object = object;

        m_requestId = m_property.object().connection()->getTableModel(m_object,
          [this, spinner](const TableModelPtr& tableModel, Message::ErrorCode ec)
          {
            if(tableModel)
            {
              m_requestId = Connection::invalidRequestId;

              m_tableWidget->setTableModel(tableModel);
              m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
              connect(m_tableWidget->selectionModel(), &QItemSelectionModel::selectionChanged,
                [this](const QItemSelection&, const QItemSelection&)
                {
                  m_buttons->button(QDialogButtonBox::Ok)->setEnabled(m_tableWidget->selectionModel()->selectedRows().count() == 1);
                });
              connect(m_tableWidget, &TableWidget::doubleClicked,
                [this](const QModelIndex& index)
                {
                  m_property.setByObjectId(m_tableWidget->getRowObjectId(index.row()));
                  accept();
                });

              delete spinner;
            }
            else
              static_cast<QVBoxLayout*>(this->layout())->insertWidget(0, AlertWidget::error(errorCodeToText(ec)));
          });
      }
      else
        static_cast<QVBoxLayout*>(this->layout())->insertWidget(0, AlertWidget::error(errorCodeToText(ec)));
    });
}