//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "widget/collapse_button.hpp"

#include <QPropertyAnimation>

CollapseButton::CollapseButton(QWidget* parent, int max_content_height, int animation_duration) :
  QToolButton(parent),
  m_max_content_height(max_content_height),
  m_animation_duration(animation_duration),
  m_content(),
  m_animator(),
  m_animation(),
  m_parent_animation()
{
  setCheckable(true);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  setStyleSheet("border:none; outline:none;");
  setIconSize(QSize(12, 12));
  setArrowType(Qt::ArrowType::RightArrow);

  QObject::connect(this, &QToolButton::toggled, this, &CollapseButton::on_toggle);
}

void
CollapseButton::set_text(const QString& text)
{
  setText(" " + text);
}

void
CollapseButton::set_content(QWidget* content)
{
  assert(content);
  m_content = content;

  m_animation = new QPropertyAnimation(m_content, "maximumHeight");
  m_animation->setDuration(m_animation_duration);
  m_animation->setEasingCurve(QEasingCurve::InOutQuad);
  m_animator.addAnimation(m_animation);

  if (!isChecked())
    m_content->setMaximumHeight(0);
}

void
CollapseButton::on_toggle(bool checked)
{
  setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
  if (m_content)
  {
    if (checked)
    {
      // Show content
      m_animation->setStartValue(0);
      m_animation->setEndValue(m_max_content_height + 10);
    }
    else
    {
      // Hide content
      m_animation->setStartValue(m_max_content_height + 10);
      m_animation->setEndValue(0);
    }

    m_animator.start();
  }
}
