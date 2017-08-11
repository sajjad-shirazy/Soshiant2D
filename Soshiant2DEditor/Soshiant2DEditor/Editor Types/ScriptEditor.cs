using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Soshiant2DEditor.Engine_Types;

namespace Soshiant2DEditor.Editor_Types
{
    public partial class ScriptEditor : Form
    {
        Sprite sprite;
        public ScriptEditor(Sprite sprite)
        {
            InitializeComponent();
            this.sprite = sprite;
        }

        private void ScriptEditor_Load(object sender, EventArgs e)
        {
            this.syntaxEditor1.Document.LoadLanguageFromXml(Application.StartupPath+"\\Syntax.xml", 0);
            this.syntaxEditor1.Document.Text = this.sprite.script;
        }

        private void ScriptEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.syntaxEditor1.Document.Text != this.sprite.script)
            {
                if (MessageBox.Show("نمي خواي ذخيره كني؟","", MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    this.sprite.script = this.syntaxEditor1.Document.Text;
                }
            }
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            this.sprite.script = this.syntaxEditor1.Document.Text;
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            this.syntaxEditor1.Document.Text = "";
        }
    }
}
