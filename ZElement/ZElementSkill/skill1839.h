#ifndef __CPPGEN_GNET_SKILL1839
#define __CPPGEN_GNET_SKILL1839
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1839:public Skill
    {
      public:
        enum
        { SKILL_ID = 1839 };
          Skill1839 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1839Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus (3 + 6 * skill->GetLevel () + 300 * skill->GetT6 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1839Stub ():SkillStub (1839)
        {
            occupation = 153;
            name = L"ÖØ»÷";
            nativename = "ÖØ»÷";
            icon = "ÖØ»÷.dds";
            maxlevel = 20;
            maxlearn = 6;
            type = 1;
            eventflag = 0;
            baseid = 0;
            basesp = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 1;
            credittype = 0;
            action[0] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[1] = "ÇàÔÆÃÅ_Ç¬À¤Õ¶";
            action[2] = "¹íÍõ×Ú_Ç¬À¤Õ¶";
            action[3] = "ºÏ»¶ÅÉ_Ç¬À¤Õ¶";
            action[4] = "ÌìÒôËÂ_Ç¬À¤Õ¶";
            action[5] = "ºÏ»¶ÅÉ_Ç¬À¤Õ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[14] = "ÇàÔÆÃÅ_Æï³Ë_Ç¬À¤Õ¶";
            action[15] = "¹íÍõ×Ú_Æï³Ë_Ç¬À¤Õ¶";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Ç¬À¤Õ¶";
            action[17] = "ÌìÒôËÂ_Æï³Ë_Ç¬À¤Õ¶";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Ç¬À¤Õ¶";
            action[19] = "";
            action[20] = "";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
#ifdef _SKILL_SERVER
            skill_limit = 0;
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1839Stub ()
        {
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (8);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 1, 2, 3, 5, 7, 10 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 2);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 8, 3 + 6 * skill->GetLevel (), 2);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 3);
        }
#endif
    };
}
#endif
