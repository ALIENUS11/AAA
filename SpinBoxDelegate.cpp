#include "SpinBoxDelegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject* parent)
	: QStyledItemDelegate(parent) {}

QWidget* SpinBoxDelegate::createEditor(QWidget* parent,
	const QStyleOptionViewItem&/*option*/,
	const QModelIndex&/*index*/) const {
	// 创建 QSpinBox 作为编辑器
	QSpinBox* editor = new QSpinBox(parent);
	editor->setMinimum(1); // 设置最小值
	editor->setMaximum(100); // 设置最大值
	return editor;
}

void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
	// 从模型中获取数据并设置到编辑器
	int value = index.model()->data(index, Qt::EditRole).toInt();
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
	if (spinBox) {
		spinBox->setValue(value);
	}
}

void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
	const QModelIndex& index) const {
	// 将编辑器的数据设置到模型中
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
	if (spinBox) {
		model->setData(index, spinBox->value(), Qt::EditRole);
	}
}

void SpinBoxDelegate::updateEditorGeometry(QWidget* editor,
	const QStyleOptionViewItem& option,
	const QModelIndex&/*index*/) const {
	// 设置编辑器的显示区域
	if (editor) {
		editor->setGeometry(option.rect);
	}
}
