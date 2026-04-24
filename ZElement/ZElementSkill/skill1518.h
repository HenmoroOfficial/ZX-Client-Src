#ifndef __CPPGEN_GNET_SKILL1518
#define __CPPGEN_GNET_SKILL1518
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1518:public Skill
    {
      public:
        enum
        { SKILL_ID = 1518 };
          Skill1518 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1518Stub:public SkillStub
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
                skill->SetRatio (1);
                skill->SetPlus (0.02 * (skill->GetPlayer ()->GetMp () + skill->GetPlayer ()->GetHp ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1518Stub ():SkillStub (1518)
        {
            occupation = 129;
            name = L"ÇéÒâÃàÃàµ¶";
            nativename = "ÇéÒâÃàÃàµ¶";
            icon = "¾öÒâ.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[1] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[2] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[3] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[4] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[5] = "¹íÍõ×Ú_ÎÞÍýÈÐ";
            action[6] = "·ÙÏã¹È_³õ¼¶»ð¹¥";
            action[7] = "ÊÞÉñ_ÃÍ»÷";
            action[8] = "ºüÑý_ÃÍ»÷";
            action[9] = "ÐùÔ¯_ÃÍ»÷";
            action[10] = "»³¹â_ÃÍ»÷";
            action[11] = "Ì«ê»_´¸»÷";
            action[12] = "Ìì»ª_ÃÍ»÷";
            action[13] = "ÁéÙí_ÃÍ»÷";
            action[14] = "Ó¢ÕÐ_ÃÍ»÷";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÎÞÍýÈÐ";
            action[21] = "·ÙÏã¹È_Æï³Ë_³õ¼¶»ð¹¥";
            action[22] = "ÊÞÉñ_Æï³Ë_ÃÍ»÷";
            action[23] = "ºüÑý_Æï³Ë_ÃÍ»÷";
            action[24] = "ÐùÔ¯_Æï³Ë_ÃÍ»÷";
            action[25] = "»³¹â_Æï³Ë_ÃÍ»÷";
            action[26] = "Ì«ê»_Æï³Ë_´¸»÷";
            action[27] = "Ìì»ª_Æï³Ë_ÃÍ»÷";
            action[28] = "ÁéÙí_Æï³Ë_ÃÍ»÷";
            action[29] = "Ó¢ÕÐ_Æï³Ë_ÃÍ»÷";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill1518Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (600);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 900000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, INT (600), 2, 2, 5, 15);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetAmount (3 * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            skill->GetVictim ()->SetProbability (1.0 * (999));
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetTime (120000);
            skill->GetVictim ()->SetRatio (0.15);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetDecanti (1);
            return true;
        }
#endif
    };
}
#endif
