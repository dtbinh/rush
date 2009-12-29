using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

namespace rtSprite
{
	/// <summary>
	/// Summary description for DropTarget.
	/// </summary>
	public class DropTarget : System.Windows.Forms.Form
	{
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.ComboBox FormatTo;
        private System.Windows.Forms.Panel DropPanel;
        private System.Windows.Forms.Label Invite;
        private System.Windows.Forms.ProgressBar Progress;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private System.Windows.Forms.ProgressBar FrameProgress;

        private  SpritePackage SPack = new SpritePackage();

		public DropTarget()
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
            this.label2 = new System.Windows.Forms.Label();
            this.FormatTo = new System.Windows.Forms.ComboBox();
            this.DropPanel = new System.Windows.Forms.Panel();
            this.FrameProgress = new System.Windows.Forms.ProgressBar();
            this.Progress = new System.Windows.Forms.ProgressBar();
            this.Invite = new System.Windows.Forms.Label();
            this.DropPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(24, 108);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 16);
            this.label2.TabIndex = 21;
            this.label2.Text = "To:";
            // 
            // FormatTo
            // 
            this.FormatTo.BackColor = System.Drawing.SystemColors.Info;
            this.FormatTo.Items.AddRange(new object[] {
                                                          "ARGB4444",
                                                          "ARGB8888",
                                                          "RGB565",
                                                          "RGB888",
                                                          "A8"});
            this.FormatTo.Location = new System.Drawing.Point(56, 104);
            this.FormatTo.Name = "FormatTo";
            this.FormatTo.Size = new System.Drawing.Size(83, 21);
            this.FormatTo.TabIndex = 20;
            this.FormatTo.Text = "ARGB4444";
            // 
            // DropPanel
            // 
            this.DropPanel.AllowDrop = true;
            this.DropPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.DropPanel.Controls.Add(this.FrameProgress);
            this.DropPanel.Controls.Add(this.Progress);
            this.DropPanel.Controls.Add(this.Invite);
            this.DropPanel.Location = new System.Drawing.Point(8, 8);
            this.DropPanel.Name = "DropPanel";
            this.DropPanel.Size = new System.Drawing.Size(128, 88);
            this.DropPanel.TabIndex = 22;
            this.DropPanel.DragDrop += new System.Windows.Forms.DragEventHandler(this.panel1_DragDrop);
            this.DropPanel.DragOver += new System.Windows.Forms.DragEventHandler(this.DropPanel_DragOver);
            // 
            // FrameProgress
            // 
            this.FrameProgress.Location = new System.Drawing.Point(7, 59);
            this.FrameProgress.Name = "FrameProgress";
            this.FrameProgress.Size = new System.Drawing.Size(112, 8);
            this.FrameProgress.TabIndex = 2;
            this.FrameProgress.Visible = false;
            // 
            // Progress
            // 
            this.Progress.Location = new System.Drawing.Point(7, 72);
            this.Progress.Name = "Progress";
            this.Progress.Size = new System.Drawing.Size(112, 8);
            this.Progress.TabIndex = 1;
            this.Progress.Visible = false;
            // 
            // Invite
            // 
            this.Invite.Location = new System.Drawing.Point(8, 16);
            this.Invite.Name = "Invite";
            this.Invite.Size = new System.Drawing.Size(96, 24);
            this.Invite.TabIndex = 0;
            this.Invite.Text = "Drag-n-drop .rspr files here";
            this.Invite.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // DropTarget
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(144, 134);
            this.Controls.Add(this.DropPanel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.FormatTo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "DropTarget";
            this.ShowInTaskbar = false;
            this.Text = ".rspr convertor";
            this.TopMost = true;
            this.DropPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }
		#endregion

        private void panel1_DragDrop(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(DataFormats.FileDrop)) return;
            string[] filenames = (string[]) e.Data.GetData(DataFormats.FileDrop);
            int nFiles = filenames.Length;

            ColorFormat dstFmt = (ColorFormat)FormatTo.SelectedIndex;

            Invite.Visible = false;
            FrameProgress.Visible = true;
            Progress.Visible = true;
            Progress.Minimum = 0;
            Progress.Maximum = nFiles;
            Progress.Step = 1;
            Progress.Value = 0;
            foreach( string s in filenames ) 
            {
                if (Path.GetExtension( s ) == ".rbs" || Path.GetExtension( s ) == ".rspr")
                {
                    FileStream ins = File.OpenRead( s );
                    if (ins == null) continue;
                    SPack.Clear();
                    SPack.Read( ins, Path.GetFileNameWithoutExtension( s ) );
                    ins.Close();
                    
                    FrameProgress.Minimum = 0;
                    FrameProgress.Maximum = SPack.Sprites.Count;
                    FrameProgress.Step = 1;
                    FrameProgress.Value = 0;
                    
                    foreach (Sprite sp in SPack.Sprites)
                    {
                        FrameProgress.Value++;
                        FrameProgress.Update();
                        sp.Convert( dstFmt );
                    }

                    File.Move( s, s + ".bak" );
                    FileStream outs = File.OpenWrite( s );
                    if (outs == null) continue;
                    SPack.Write( outs );
                    outs.Close();
                }
                Progress.Value++;
                Progress.Update();
            }
            Invite.Visible = true;
            Progress.Visible = false;
            FrameProgress.Visible = false;
        }

        private void DropPanel_DragOver(object sender, System.Windows.Forms.DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.All;
            }
        }
	}
}
