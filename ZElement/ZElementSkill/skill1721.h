#ifndef __CPPGEN_GNET_SKILL1721
#define __CPPGEN_GNET_SKILL1721
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1721:public Skill
    {
      public:
        enum
        { SKILL_ID = 1721 };
          Skill1721 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1721Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
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
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0);
                skill->SetPlus (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1721Stub ():SkillStub (1721)
        {
            occupation = 129;
            name = L"½µÄ§½ğèÆ";
            nativename = "½µÄ§½ğèÆ";
            icon = "½ğ¸Õ½µÄ§.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[1] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[2] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[3] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[4] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[5] = "ÌìÒôËÂ_·¨±¦_½ğ¸ÕèÆ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[16] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[17] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[18] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[20] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_½ğ¸ÕèÆ";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill1721Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (24);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetRatio ((skill->GetPlayer ()->GetTmplid () == 19691 || skill->GetPlayer ()->GetTmplid () == 19692) ? 999 : 0);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetTmplid () == 19691 || skill->GetPlayer ()->GetTmplid () == 19692) ? 1 : 0);
            skill->GetVictim ()->SetSecondattack (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
