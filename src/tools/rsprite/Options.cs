using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace rtSprite
{
	/// <summary>
	/// Summary description for Options.
	/// </summary>
	public class Options : System.Windows.Forms.Form
	{
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.NumericUpDown spMaxQuadSide;
        private System.Windows.Forms.Label label1;
        public System.Windows.Forms.NumericUpDown spMinQuadSide;
        private System.Windows.Forms.Button btnOK;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

        public int MinQuadSide = 16;
        public int MaxQuadSide = 256;

		public Options()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			spMinQuadSide.Value = MinQuadSide;
            spMaxQuadSide.Value = MaxQuadSide;
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.spMaxQuadSide = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.spMinQuadSide = new System.Windows.Forms.NumericUpDown();
            this.btnOK = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.spMaxQuadSide)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.spMinQuadSide)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.spMaxQuadSide);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.spMinQuadSide);
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(168, 88);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Packing";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(16, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(88, 16);
            this.label2.TabIndex = 7;
            this.label2.Text = "Max Quad Side:";
            // 
            // spMaxQuadSide
            // 
            this.spMaxQuadSide.Location = new System.Drawing.Point(104, 52);
            this.spMaxQuadSide.Maximum = new System.Decimal(new int[] {
                                                                          256,
                                                                          0,
                                                                          0,
                                                                          0});
            this.spMaxQuadSide.Minimum = new System.Decimal(new int[] {
                                                                          16,
                                                                          0,
                                                                          0,
                                                                          0});
            this.spMaxQuadSide.Name = "spMaxQuadSide";
            this.spMaxQuadSide.Size = new System.Drawing.Size(48, 20);
            this.spMaxQuadSide.TabIndex = 6;
            this.spMaxQuadSide.Value = new System.Decimal(new int[] {
                                                                        256,
                                                                        0,
                                                                        0,
                                                                        0});
            this.spMaxQuadSide.ValueChanged += new System.EventHandler(this.spMaxQuadSide_ValueChanged);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(16, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "Min Quad Side:";
            // 
            // spMinQuadSide
            // 
            this.spMinQuadSide.Location = new System.Drawing.Point(104, 20);
            this.spMinQuadSide.Maximum = new System.Decimal(new int[] {
                                                                          256,
                                                                          0,
                                                                          0,
                                                                          0});
            this.spMinQuadSide.Minimum = new System.Decimal(new int[] {
                                                                          16,
                                                                          0,
                                                                          0,
                                                                          0});
            this.spMinQuadSide.Name = "spMinQuadSide";
            this.spMinQuadSide.Size = new System.Drawing.Size(48, 20);
            this.spMinQuadSide.TabIndex = 4;
            this.spMinQuadSide.Value = new System.Decimal(new int[] {
                                                                        16,
                                                                        0,
                                                                        0,
                                                                        0});
            this.spMinQuadSide.ValueChanged += new System.EventHandler(this.spMinQuadSide_ValueChanged);
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(52, 112);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(80, 24);
            this.btnOK.TabIndex = 5;
            this.btnOK.Text = "OK";
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // Options
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(184, 144);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Options";
            this.Text = "Options";
            this.TopMost = true;
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.spMaxQuadSide)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.spMinQuadSide)).EndInit();
            this.ResumeLayout(false);

        }
		#endregion

        private void spMinQuadSide_ValueChanged(object sender, System.EventArgs e)
        {
            int val = (int)spMinQuadSide.Value;
            if (BitOps.IsPow2( (long)val )) return;
            if (val > MinQuadSide)
            {
                MinQuadSide = (int)BitOps.NextPow2( (long)val );
            }
            else
            {
                MinQuadSide = (int)BitOps.PrevPow2( (long)val );
            }
            if (MinQuadSide < RectPartition.c_MinQuadSide) 
            {
                MinQuadSide = RectPartition.c_MinQuadSide;
            }
            spMinQuadSide.Value = MinQuadSide;
        }

        private void spMaxQuadSide_ValueChanged(object sender, System.EventArgs e)
        {
            int val = (int)spMaxQuadSide.Value;
            if (BitOps.IsPow2( (long)val )) return;
            if (val > MaxQuadSide)
            {
                MaxQuadSide = (int)BitOps.NextPow2( (long)val );
            }
            else
            {
                MaxQuadSide = (int)BitOps.PrevPow2( (long)val );
            }
            if (MaxQuadSide > RectPartition.c_MaxQuadSide) 
            {
                MaxQuadSide = RectPartition.c_MaxQuadSide;
            }
            spMaxQuadSide.Value = MaxQuadSide;
        }

        private void btnOK_Click(object sender, System.EventArgs e)
        {
            Close();
        }
	}
}
