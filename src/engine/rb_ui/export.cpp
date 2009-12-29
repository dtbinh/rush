#include "stdafx.h"
#include "JReflect.h"

export(rb_ui)
{
    link_class  ( JWidget           );
    link_class  ( JWindowServer     );
    link_class  ( JDialog           );
    link_class  ( JButton           );
    link_class  ( JPolyButton       );
    link_class  ( JLabel            );

    link_class  ( JEditBox          );
    link_class  ( JPicture          );

    link_class  ( JPanel            );
    link_class  ( JPopupPanel       );
    link_class  ( JGrid             );
    link_class  ( JCheckBox         );
    link_class  ( JCheckGroup       );
    link_class  ( JListBox          );
    link_class  ( JPopupMenu        );
    link_class  ( JSlider           );
    link_class  ( JShortcut         );

    link_class  ( JComboBox         );
    link_class  ( JScrollBar        );
    link_class  ( JScrollBox        );

    link_class  ( JProgress         );
    link_class  ( JSwitch           );
    link_class  ( JWidgetRef        );
    link_class  ( JStringSelector   );
}