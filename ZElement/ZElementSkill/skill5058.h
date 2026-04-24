#ifndef __CPPGEN_GNET_SKILL5058
#define __CPPGEN_GNET_SKILL5058
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5058:public Skill
    {
      public:
        enum
        { SKILL_ID = 5058 };
          Skill5058 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5058Stub:public SkillStub
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
            bool Cancel (Skill * skill) const
            {
                return 1;
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
                skill->SetRatio (0 + 0.15 * 2);
                skill->SetPlus ((0.8 + 0.1 * 8) * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetMp (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5058Stub ():SkillStub (5058)
        {
            occupation = 128;
            name = L"¾ÛÁéÖä";
            nativename = "¾ÛÁéÖä";
            icon = "¾ÛÁéÖä.dds";
            maxlevel = 1;
            maxlearn = 1;
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
            skill_class = 0;
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
            action[0] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[1] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[2] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[3] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[4] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[5] = "¹íÍõ×Ú_¾ÛÁéÖä";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¾ÛÁéÖä";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill5058Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 75000 - 10000 * 2;
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
            return _snwprintf (buffer, length, format, 80 + 10 * 8);

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
            skill->GetVictim ()->SetTime (500);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetMaxmp ());
            skill->GetVictim ()->SetValue (1);
            skill->GetVictim ()->SetMpgen (1);
            return true;
        }
#endif
    };
}
#endif
