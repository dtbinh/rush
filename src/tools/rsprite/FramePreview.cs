using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace rtSprite
{
	/// <summary>
	/// Summary description for FramePreview.
	/// </summary>
	public class FramePreview : System.Windows.Forms.Form
	{
        public System.Windows.Forms.PictureBox ThumbBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        public Sprite CurSprite = new Sprite();


		public FramePreview()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.ThumbBox = new System.Windows.Forms.PictureBox();
            this.SuspendLayout();
            // 
            // ThumbBox
            // 
            this.ThumbBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
                | System.Windows.Forms.AnchorStyles.Left) 
                | System.Windows.Forms.AnchorStyles.Right)));
            this.ThumbBox.Location = new System.Drawing.Point(0, 0);
            this.ThumbBox.Name = "ThumbBox";
            this.ThumbBox.Size = new System.Drawing.Size(296, 280);
            this.ThumbBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.ThumbBox.TabIndex = 0;
            this.ThumbBox.TabStop = false;
            this.ThumbBox.Paint += new System.Windows.Forms.PaintEventHandler(this.ThumbBox_Paint);
            this.ThumbBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ThumbBox_MouseDown);
            // 
            // FramePreview
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Controls.Add(this.ThumbBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FramePreview";
            this.Text = "FramePreview";
            this.TopMost = true;
            this.Closed += new System.EventHandler(this.FramePreview_Closed);
            this.ResumeLayout(false);

        }
		#endregion

        private void ThumbBox_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            int pivotX = (int)CurSprite.PivotX;
            int pivotY = (int)CurSprite.PivotY;
            int shiftX = (int)CurSprite.ShiftX;
            int shiftY = (int)CurSprite.ShiftY;

            //  draw shift point
            Pen shiftPen = new Pen( Color.Blue, 1 );
            e.Graphics.DrawLine( shiftPen, shiftX - 3, shiftY, shiftX + 3, shiftY );
            e.Graphics.DrawLine( shiftPen, shiftX, shiftY - 3, shiftX, shiftY + 3 );

            //  draw pivot
            Pen pivotPen = new Pen( Color.Red, 1 );
            e.Graphics.DrawLine( pivotPen, pivotX - 3, pivotY, pivotX + 3, pivotY );
            e.Graphics.DrawLine( pivotPen, pivotX, pivotY - 3, pivotX, pivotY + 3 );

            //  draw quads
            if (!MainForm.Instance.ShowQuads.Checked) return;
            Pen pen = new Pen( Color.Red, 1 );
            SolidBrush br = new SolidBrush( Color.FromArgb( 0x22FF0000 ) );
            for (int i = 0; i < CurSprite.Partition.Rects.Count; i++)
            {
                Frame rc = (Frame)CurSprite.Partition.Rects[i];
                e.Graphics.DrawRectangle( pen, rc.x, rc.y, rc.w, rc.h );
            }
        }

        private void FramePreview_Closed(object sender, System.EventArgs e)
        {
            MainForm.Instance.ItemPreview.Checked = false;
            MainForm.Instance.ShowQuads.Enabled = false;
        }

        private void ThumbBox_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                CurSprite.PivotX = (short)e.X;
                CurSprite.PivotY = (short)e.Y;
                CurSprite.ShiftX = (short)e.X;
                CurSprite.ShiftY = (short)e.Y;
            }
            if (e.Button == MouseButtons.Right)
            {
                CurSprite.ShiftX = (short)e.X;
                CurSprite.ShiftY = (short)e.Y;
            }
            MainForm.Instance.UpdateSpriteInfo();
        }
	}
}
