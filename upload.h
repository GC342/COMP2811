#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>

#include <QFileDialog>
#include <QWidget>
#include <QPushButton>
#include <QTreeWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QLayout>
#include <QTextEdit>


class UploadWidget : public QWidget {
private:
    QWidget* m_pRow0;
    QWidget* m_pRow1;
    QWidget* m_pRow2;
    QWidget* m_pRow3;

    QPushButton* m_pFilePathButton; // read-only
    QLineEdit* m_pTitleBox;
    QLineEdit* m_pTagSelector;
    QTextEdit* m_pDescription;
    QPushButton* m_pUploadButton;

    QLayout* m_pLayout;

    // todo: add layout like so
    // file picker         |
    // title | tag selector|
    // desc----------------|
    // --------------------|
    //                 add |

    // so
    // qvbox
    //  - qhbox (file picker)
    //  - qhbox
    //   - title | tag
    //  - qhbox
    //   - desc
    //  - qhbox
    //   - add

    void setupFilePicker() {
        m_pRow0 = new QWidget(this);
        auto pRowLayout = new QHBoxLayout(m_pRow0);
        pRowLayout->setMargin(3);

        m_pFilePathButton = new QPushButton(m_pRow0);
        m_pFilePathButton->setText("Click to select a video file to upload...");

        m_pFilePathButton->connect(m_pFilePathButton, SIGNAL(&QPushButton::released), this, SLOT(&UploadWidget::fileBoxClicked));

        m_pRow0->setLayout(pRowLayout);
        pRowLayout->addWidget(m_pFilePathButton);

        // probably?
        m_pLayout->addWidget(m_pRow0);
    }

    void setupTitleAndTags() {
        m_pRow1 = new QWidget(this);
        auto pRowLayout = new QHBoxLayout(m_pRow1);
        pRowLayout->setMargin(3);

        m_pTitleBox = new QLineEdit(m_pRow1);
        m_pTitleBox->setPlaceholderText("Enter title...");

        m_pTagSelector = new QLineEdit(m_pRow1);

        // todo: make this
        m_pTagSelector->setPlaceholderText("Enter tags separated by comma...");
        {
            QStringList tags = {"skiing", "hiking", "cycling", "swimming", "running", "skydiving"};
            QCompleter* pCompleter = new QCompleter(tags, m_pRow1);
            pCompleter->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
            m_pTagSelector->setCompleter(pCompleter);
        }
        pRowLayout->addWidget(m_pTitleBox);
        pRowLayout->addWidget(m_pTagSelector);

        m_pLayout->addWidget(m_pRow1);
    }

    void setupDescription() {
        m_pRow2 = new QWidget(this);
        auto pRowLayout = new QHBoxLayout(m_pRow2);
        pRowLayout->setMargin(3);

        m_pDescription = new QTextEdit();
        m_pDescription->setPlaceholderText("Description...");

        pRowLayout->addWidget(m_pDescription);
        m_pLayout->addWidget(m_pRow2);
    }

    void setupAddButton() {
        m_pRow3 = new QWidget(this);
        auto pRowLayout = new QHBoxLayout(m_pRow3);

        m_pUploadButton = new QPushButton(m_pRow3);
        m_pUploadButton->setText("Add to library");

        m_pUploadButton->setStyleSheet("float: right; left: -1px; max-width: 120px;");

        pRowLayout->addWidget(m_pUploadButton);


        m_pLayout->addWidget(m_pRow3);
    }

public slots:
    void fileBoxClicked() {
        QString filename = QFileDialog::getOpenFileName(this, "Select video file", QDir::currentPath());
        if (!filename.isEmpty()) {
            if (m_pFilePathButton->text().contains(filename) == -1) {
                m_pFilePathButton->setText(filename);
            }
        }
    }
public:
    UploadWidget(QWidget* pParent) : QWidget(pParent) {
        m_pLayout = new QVBoxLayout(this);
        this->setMaximumSize(pParent->maximumSize());

        setupFilePicker();
        setupTitleAndTags();
        setupDescription();
        setupAddButton();
    }
};

#endif // UPLOAD_H
